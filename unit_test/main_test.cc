//
// Created by sleeplessy on 11/3/18.
//


#include <glfwxx/instance_manager.hpp>
#include <iostream>
#include <GLFW/glfw3.h>
#include <thread>

int main() {
    glfw::instance_manager manager;
    auto window = manager.get_window_manager().create_window(400, 800, "test1");
    manager.get_window_manager().add_window_callback(window,[&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    auto window2 = manager.get_window_manager().create_window(400, 800, "test2", [&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    manager.get_window_manager().add_window_callback(window2,[](glfw::window_ptr_t ptr){
        std::cout << "test!!!" << std::endl;
    });

    for (auto &x : manager.get_window_manager().get_pool()) {
        x.second->show();
    }

    glfw::window_id_t id(0);
    id.set_manager(manager.get_window_manager());
    std::cout << id.get_window()->get_title() << std::endl;
    window.get_window()->get_focus();

    glfw::window_id_t id2(2);
    try {
        id2.get_window();
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    manager.poll();
    return 0;
}
