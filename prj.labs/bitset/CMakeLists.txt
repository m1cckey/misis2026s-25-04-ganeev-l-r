add_library(bitset
    BitsetD.cpp
    BitsetD.hpp
)

target_include_directories(bitset PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bitset PUBLIC DiostR)
