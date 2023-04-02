include(GoogleTest)

foreach (test_abs_filename ${TEST_SRC_LIST})
    get_filename_component(test_filename ${test_abs_filename} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    add_executable(
        ${test_name}
        ${test_abs_filename}
    )
    target_link_libraries(
        ${test_name}
        ${PROJECT_NAME}
        GTest::gtest_main
    )
    gtest_discover_tests(${test_name})

    add_custom_target(
        run_${test_name} 
        test/${test_name}
        DEPENDS ${test_name}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach ()