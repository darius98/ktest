cmake_minimum_required(VERSION 3.7)

GET_PROPERTY(__LOCAL_KKTEST_LIBRARY_INCLUDE_GUARD__ GLOBAL PROPERTY __KKTEST_LIBRARY_INCLUDE_GUARD__)
if(__LOCAL_KKTEST_LIBRARY_INCLUDE_GUARD__)
    return()
endif()
SET_PROPERTY(GLOBAL PROPERTY __KKTEST_LIBRARY_INCLUDE_GUARD__ TRUE)

function(add_kktest TEST_NAME)
    if (NOT TESTS_PATH)
        message(FATAL_ERROR "TESTS_PATH variable not set.")
    endif ()

    find_library(KKTEST_LIB kktest)
    if (NOT KKTEST_LIB)
        message(FATAL_ERROR "kktest library not installed.")
    endif()

    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})

    add_executable(${ARGV})
    install(TARGETS ${TEST_NAME} DESTINATION ${TEST_NAME})
    target_link_libraries(${TEST_NAME} kktest)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()