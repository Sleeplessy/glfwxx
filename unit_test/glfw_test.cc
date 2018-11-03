//
// Created by sleeplessy on 11/3/18.
//

#include <GLFW/glfw3.h>
#include <iostream>
int main()
{

    if(!glfwInit())
    {
        std::cout << "error";
    }
    auto window = glfwCreateWindow(400,400,"test", nullptr, nullptr);
    glfwShowWindow(window);
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}