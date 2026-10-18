add_executable(test_minib64
  test_minib64.c
)

target_link_libraries(test_minib64
  PRIVATE
    minib64
    unity
)

add_test(
  NAME test_minib64
  COMMAND test_minib64
)