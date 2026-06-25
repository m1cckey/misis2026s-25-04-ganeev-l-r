add_executable(rational_test rational_test.cpp)
target_link_libraries(rational_test rational)

add_executable(arrayd_test arrayd_test.cpp)
target_link_libraries(arrayd_test arrayd)

add_executable(bitsetd_test bitsetd_test.cpp)
target_link_libraries(bitsetd_test bitsetd)

add_executable(jaggeda_test jaggeda_test.cpp)
target_link_libraries(jaggeda_test jaggeda)

add_test(NAME rational_test COMMAND rational_test)
add_test(NAME arrayd_test COMMAND arrayd_test)
add_test(NAME bitsetd_test COMMAND bitsetd_test)
add_test(NAME jaggeda_test COMMAND jaggeda_test)
