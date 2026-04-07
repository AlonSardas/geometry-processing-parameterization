add_executable(run_tests
  test_lscm.cpp
  test_tutte.cpp
  test_vector_area_matrix.cpp
)

target_link_libraries(run_tests
  core
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
