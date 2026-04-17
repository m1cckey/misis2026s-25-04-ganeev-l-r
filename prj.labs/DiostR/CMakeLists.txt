add_library(DiostR
    DiostR.cpp
    DiostR.hpp
)

target_include_directories(DiostR PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
