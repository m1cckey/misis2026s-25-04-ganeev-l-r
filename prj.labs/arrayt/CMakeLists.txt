add_library(arrayt
    arrayt.cpp
    arrayt.hpp
)

target_include_directories(arrayt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
