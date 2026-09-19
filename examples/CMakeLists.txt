add_executable(minib64_example
  example_1.c
)

target_link_libraries(minib64_example
  PRIVATE
    minib64
)
