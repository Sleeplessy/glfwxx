//
// Created by sleeplessy on 11/3/18.
//

#ifndef GLFWXX_COMMON_HPP
#define GLFWXX_COMMON_HPP

// Bring up GLFW headers
#ifdef GLFWXX_INCLUDE_GLFW
#include <GLFW/glfw3.h>
#endif

#ifndef GLFW_VERSION_MAJOR
struct GLFWwindow;
struct GLFWmonitor;
#endif
namespace std{
    // for the thread class
    class thread;
}
#endif //GLFWXX_COMMON_HPP
