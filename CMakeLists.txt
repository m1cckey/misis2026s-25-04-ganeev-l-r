cmake_minimum_required(VERSION 3.20)

project(ganeev-l-r-25-04)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

if(MSVC)
    add_compile_options(/EHsc)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/prj.thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}/prj.labs
)

add_subdirectory(prj.labs)
add_subdirectory(prj.test)

enable_testing()
