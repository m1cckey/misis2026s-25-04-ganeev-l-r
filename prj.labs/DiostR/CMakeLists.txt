add_library(DiostR DiostR.cpp DiostR.hpp)
target_include_directories(DiostR INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
set_target_properties(DiostR PROPERTIES PUBLIC_HEADER DiostR.hpp)
