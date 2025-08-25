set(TEST_EXE_NAME cads-tests)

add_executable(${TEST_EXE_NAME}
    vector_tests.cpp
    list_tests.cpp
    stack_tests.cpp
        queue_tests.cpp
)

target_link_libraries(${TEST_EXE_NAME}
    PRIVATE
    cads
    GTest::gtest_main
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(${TEST_EXE_NAME})