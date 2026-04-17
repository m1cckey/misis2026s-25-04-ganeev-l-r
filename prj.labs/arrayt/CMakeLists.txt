add_library(arrayt
    arrayT.cpp
    arrayT.hpp
)

target_include_directories(arrayt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
