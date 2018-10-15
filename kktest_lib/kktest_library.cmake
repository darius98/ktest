if(__KKTEST_LIBRARY_INCLUDE_GUARD__)
    return()
endif()
set(__KKTEST_LIBRARY_INCLUDE_GUARD__ TRUE)

cmake_minimum_required(VERSION 3.7)
project(KKTestLibraries)
set(CMAKE_CXX_STANDARD 14)

# Set KKTest library source directories
# Allow them to be set externally for dev purposes
if(NOT KKTEST_DIR)
    set(KKTEST_DIR "${CMAKE_CURRENT_LIST_DIR}/kktest_src")
endif()
if(NOT KKTEST_SRC_DIR)
    set(KKTEST_SRC_DIR "${KKTEST_DIR}/src")
endif()
if(NOT KKTEST_THIRD_PARTY_DIR)
    set(KKTEST_THIRD_PARTY_DIR "${KKTEST_DIR}/third_party")
endif()

set(AUTOJSON_DIR "${KKTEST_THIRD_PARTY_DIR}/autojson")
set(AUTOJSON_INCLUDE_DIR "${AUTOJSON_DIR}/include")
file(GLOB AUTOJSON_SRC ${AUTOJSON_DIR}/src/*.cpp)
add_library(autojson ${AUTOJSON_SRC})
target_compile_definitions(autojson PUBLIC autojsonuselib)
target_include_directories(autojson PUBLIC ${AUTOJSON_INCLUDE_DIR})

set(EASYFLAGS_DIR "${KKTEST_THIRD_PARTY_DIR}/easyflags")
set(EASYFLAGS_INCLUDE_DIR "${EASYFLAGS_DIR}/include")
add_library(easyflags "${EASYFLAGS_DIR}/src/EasyFlags.cpp")
target_compile_definitions(easyflags PUBLIC easyflagsuselib)
target_include_directories(easyflags PUBLIC ${EASYFLAGS_INCLUDE_DIR})
target_link_libraries(easyflags autojson)

file(GLOB KKTestLibImpl ${KKTEST_SRC_DIR}/testing/*.cpp)
add_library(kktest ${KKTestLibImpl})
target_include_directories(kktest PUBLIC ${KKTEST_SRC_DIR})
target_link_libraries(kktest autojson easyflags)

file(GLOB KKTestMatchersLibImpl ${KKTEST_SRC_DIR}/matcher/*.cpp)
add_library(kktest_matchers ${KKTestMatchersLibImpl})
target_include_directories(kktest_matchers PUBLIC ${KKTEST_SRC_DIR})
target_link_libraries(kktest_matchers kktest)

function(testcase TEST_NAME FILE_NAME)
    if (NOT TESTS_PATH)
        message(FATAL_ERROR "TESTS_PATH variable not set.")
    endif ()

    message(STATUS "KKTEST::TEST_CASE ${TESTS_PATH}/${TEST_NAME}")

    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})

    foreach(arg IN LISTS ARGN)
        if (TARGET ${arg})
            list(APPEND LIB_LIST ${arg})
        else()
            list(APPEND FILE_LIST ${arg})
        endif()
    endforeach()

    add_executable(${TEST_NAME} ${FILE_NAME} ${FILE_LIST})
    install(TARGETS ${TEST_NAME} DESTINATION ${TEST_NAME})
    target_link_libraries(${TEST_NAME} kktest ${LIB_LIST})

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()

function(matcher_testcase TEST_NAME FILE_NAME)
    testcase(${TEST_NAME} ${FILE_NAME} kktest_matchers ${ARGN})
endfunction()
