//
// Created by sleeplessy on 11/3/18.
//

#ifndef GLFWXX_INSTANCE_MANAGER_HPP
#define GLFWXX_INSTANCE_MANAGER_HPP

// make this header independent with others
#include <glfwxx/common.hpp>
#include <glfwxx/window.hpp>
#include <future>
namespace glfw {
    enum GLFW_INSTANCE_FLAG {
        GLFW_RUNNING,   // instance is running
        GLFW_WANT_QUIT, // instance want quit
    };

    enum GLFW_POLLING_FLAG
    {
        GLFW_USE_POLL,  // use glfwPollEvents
        GLFW_USE_WAIT,  // use glfwWaitEvents
    };
    class instance_manager {
    public:
        instance_manager();

        ~instance_manager();

        window_manager &get_window_manager();

        void set_event_flag(GLFW_POLLING_FLAG flag);

        virtual void set_vulkan_flag(const bool use_opengl = false);

        window_id_t create_window(int width, int height, std::string title = "",
                                  const std::function<void(window_ptr_t)> &&callback = nullptr);

        void poll();

    private:
        window_manager m_window_manager;
        GLFW_POLLING_FLAG m_event_flag;

        std::promise<GLFW_INSTANCE_FLAG> m_instance_flag;
        std::future<glfw::GLFW_INSTANCE_FLAG> m_quiting_flags;
    };
}

#endif //GLFWXX_INSTANCE_MANAGER_HPP
