//
// Created by sleeplessy on 11/3/18.
//

#ifndef GLFWXX_WINDOW_HPP
#define GLFWXX_WINDOW_HPP

#ifndef GLFW_VERSION_MAJOR
struct GLFWwindow;
struct GLFWmonitor;
#endif

#include <string>
#include <map>
#include <memory>
#include <functional>

namespace glfw {

    struct window_id_t;

    class window {
    public:
        window(int width, int height, std::string title = "");

        virtual ~window();

        window(window &&) = delete;

        window(window &) = delete;

        const std::string get_title() noexcept;

        GLFWwindow *data();  // get the original GLFWwindow pointer

        virtual void show();

        void update();

        void get_focus();  // focus on this window

        operator GLFWwindow *() { return raw_handle; }

    private:
        GLFWwindow *raw_handle;
        std::string title;  // window title
    };

    typedef std::shared_ptr<window> window_ptr_t;

    inline const bool operator==(window &lhs, window &rhs) {
        return lhs.data() == rhs.data();
    }


    class window_manager {
    public:
        window_manager();

        virtual ~window_manager();


        window_id_t create_window(int width, int height, std::string title = "",
                                  const std::function<void(window_ptr_t)> &callback = nullptr);

        void add_window_callback(window_id_t id, const std::function<void(window_ptr_t)> &callback);

        window_ptr_t &get_window(window_id_t id);  // find a window using it's id
        window_ptr_t &get_window(int id);  // find a window using it's id


        std::map<window_id_t, window_ptr_t> &get_pool() noexcept;

        const bool all_cleared();  // check if there isn't any window managed

        void update_all();  // update all window managed by this manager

        static window_id_t find_id(GLFWwindow *handle);  // find window id using handle
        static void invoke_close_callback(GLFWwindow *handle);

    private:
        static std::map<window_id_t, window_ptr_t> m_raw_handle_pool;
        static std::map<window_id_t, std::function<void(window_ptr_t)>> close_callback_map;
    };

    struct window_id_t {
        window_id_t(int id);

        void set_id(int id);

        void set_manager(window_manager &manager);

        window_ptr_t &get_window();

        int m_id;
        window_manager *m_manager;
    };

    inline bool operator<(const window_id_t &lhs, const window_id_t &rhs) {
        return lhs.m_id < rhs.m_id;
    }

    inline bool operator==(const window_id_t &lhs, const window_id_t &rhs)
    {
        return lhs.m_id == rhs.m_id;
    }

    inline bool operator==(window_id_t lhs, window_id_t rhs)
    {
        return lhs.m_id == rhs.m_id;
    }

}

#endif //GLFWXX_WINDOW_HPP
