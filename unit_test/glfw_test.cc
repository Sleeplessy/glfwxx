//
// Created by sleeplessy on 11/3/18.
//

#include <glfwxx/instance_manager.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
int main()
{
    glfwInit();
    GLFWwindow* wind = glfwCreateWindow(800,600,"test", nullptr, nullptr);
    glfwShowWindow(wind);
    while(!glfwWindowShouldClose(wind))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(wind);
    glfwTerminate();
    return 0;
}