add_executable(minib64_example_1
  example_1.c
)

target_link_libraries(minib64_example_1
  PRIVATE
    minib64
)

add_executable(minib64_example_2
  example_2.c
)

target_link_libraries(minib64_example_2
  PRIVATE
    minib64
)
