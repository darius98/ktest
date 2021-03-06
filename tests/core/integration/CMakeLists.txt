include(FindThreads)

add_integration_test(core_single_test_pass
        single_test_pass.py
        single_test_pass.cpp
        )

add_integration_test(core_single_test_fail
        single_test_fail.py
        single_test_fail.cpp
        )

add_integration_test(core_single_test_optional_fail
        single_test_optional_fail.py
        single_test_optional_fail.cpp
        )

add_integration_test(core_single_test_multiple_executions_pass
        single_test_multiple_executions_pass.py
        single_test_multiple_executions_pass.cpp
        )

add_integration_test(core_single_test_multiple_executions_fail
        single_test_multiple_executions_fail.py
        single_test_multiple_executions_fail.cpp
        )

add_integration_test(core_single_test_kbs
        single_test_kbs.py
        single_test_kbs.cpp
        )

add_integration_test(core_single_test_timeout
        single_test_timeout.py
        single_test_timeout.cpp
        )

add_integration_test(core_single_test_exit_0
        single_test_exit_0.py
        single_test_exit_0.cpp
        )

add_integration_test(core_single_test_exit_1
        single_test_exit_1.py
        single_test_exit_1.cpp
        )

add_integration_test(core_single_test_fail_thread
        single_test_fail_thread.py
        single_test_fail_thread.cpp
        )
target_link_libraries(mcga_test_integration_core_single_test_fail_thread_binary ${CMAKE_THREAD_LIBS_INIT})

add_integration_test(core_single_file_multiple_cases
        single_file_multiple_cases.py
        single_file_multiple_cases.cpp
        )

add_integration_test(core_multiple_files
        multiple_files.py
        multiple_files_1.cpp multiple_files_2.cpp
        )

add_integration_test(core_multiple_tests_one_fails
        multiple_tests_one_fails.py
        multiple_tests_one_fails.cpp
        )

add_integration_test(core_emit_warnings
        emit_warnings.py
        emit_warnings.cpp
        )
target_link_libraries(mcga_test_integration_core_emit_warnings_binary ${CMAKE_THREAD_LIBS_INIT})
