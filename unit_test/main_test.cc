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
    manager.get_window_manager().add_window_close_callback(window, [&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    auto window2 = manager.get_window_manager().create_window(400, 800, "test2", [&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    auto window3 = manager.get_window_manager().create_window(400, 800, "test3", [&](glfw::window_ptr_t ptr) {
        std::cout << ptr->get_title() << " destroyed" << std::endl;
    });

    manager.get_window_manager().add_window_close_callback(window2, [](glfw::window_ptr_t ptr) {
        std::cout << "test!!!" << std::endl;
    });

    window.get_window()->hide();

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


    manager.get_window_manager().add_keyboard_callback(window2,[&](glfw::window_ptr_t ptr,int key, int scancode, int action, int mods){
        if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)  // press Enter to show window1
        {
            window.get_window()->show();
        }
    });
    manager.set_event_flag(glfw::GLFW_USE_WAIT);
    manager.poll();

    for(int i = 0 ; i < 10 ; i ++)
    {
        std::cout<< "I can count while GLFW updating! " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
