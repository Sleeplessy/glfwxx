//
// Created by sleeplessy on 11/3/18.
//

#ifndef GLFWXX_INSTANCE_MANAGER_HPP
#define GLFWXX_INSTANCE_MANAGER_HPP

// make this header independent with others
#include <glfwxx/window.hpp>

namespace glfw
{
    enum GLFW_INSTANCE_FLAG
    {
        GLFW_USE_POLL,  // use glfwPollEvents
        GLFW_USE_WAIT,  // use glfwWaitEvents
    };

    class instance_manager
    {
    public:
        instance_manager();
        ~instance_manager();
        window_manager& get_window_manager();
        void set_event_flag(GLFW_INSTANCE_FLAG flag);
        window_id_t create_window(int width, int height, std::string title="",const std::function<void(window_ptr_t)> &&callback = nullptr);
        void poll();
    private:
        window_manager m_window_manager;
        GLFW_INSTANCE_FLAG m_event_flag;
    };
}

#endif //GLFWXX_INSTANCE_MANAGER_HPP
