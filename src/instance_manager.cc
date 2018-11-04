
#include <GLFW/glfw3.h>
#include <glfwxx/instance_manager.hpp>
#include <stdexcept>
#include <thread>

glfw::instance_manager::instance_manager() :
        m_event_flag{glfw::GLFW_POLLING_FLAG::GLFW_USE_POLL},
        m_instance_flag{},
        m_quiting_flags{m_instance_flag.get_future()}{
                if (!glfwInit()) {
                    throw std::runtime_error("Failed to init glfw");
                }
        }

glfw::instance_manager::~instance_manager() {
    m_quiting_flags.wait();
    glfwTerminate();
}

glfw::window_manager &glfw::instance_manager::get_window_manager() {
    return m_window_manager;
}

glfw::window_id_t glfw::instance_manager::create_window(int width, int height, std::string title,
                                                        const std::function<void(glfw::window_ptr_t)> &&callback) {
    return m_window_manager.create_window(width, height, title, callback);
}

void glfw::instance_manager::poll() {

    std::thread([&, this] {
        while (!m_window_manager.all_cleared()) {
            if (m_event_flag == glfw::GLFW_POLLING_FLAG::GLFW_USE_POLL) {
                m_window_manager.update_all();
                glfwPollEvents();
            } else {
                m_window_manager.update_all();
                glfwWaitEvents();
            }
            std::this_thread::yield();
        }
        m_instance_flag.set_value_at_thread_exit(glfw::GLFW_INSTANCE_FLAG::GLFW_WANT_QUIT);
    }).detach();
}

void glfw::instance_manager::set_event_flag(GLFW_POLLING_FLAG flag) {
    m_event_flag = flag;
}
