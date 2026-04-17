add_executable(bitset_test
    BitsetD_test.cpp
)

target_link_libraries(bitset_test PRIVATE DiostR)

target_include_directories(bitset_test PRIVATE
    ${CMAKE_SOURCE_DIR}/prj.labs/DiostR
)
