add_executable(main_test main_test.cc)
target_link_libraries(main_test glfwxx glfw)
add_executable(glfw_test glfw_test.cc)
target_link_libraries(glfw_test glfw)