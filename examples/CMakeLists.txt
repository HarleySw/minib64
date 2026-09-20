add_executable(minib64_example_encode
  example_encode.c
)

target_link_libraries(minib64_example_encode
  PRIVATE
    minib64
)

add_executable(minib64_example_decode
  example_decode.c
)

target_link_libraries(minib64_example_decode
  PRIVATE
    minib64
)
