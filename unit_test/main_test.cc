//
// Created by sleeplessy on 11/3/18.
//


#include <glfwxx/instance_manager.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <thread>

int main() {
    glfw::instance_manager manager;
    auto window = manager.get_window_manager().create_window(400, 800, "test1", [&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    auto window2 = manager.get_window_manager().create_window(400, 800, "test2", [&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    for (auto &x : manager.get_window_manager().get_pool()) {
        x.second->show();
    }

    manager.poll();

}
