//
// Created by sleeplessy on 11/3/18.
//

#include <algorithm>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glfwxx/common.hpp>
#include <glfwxx/window.hpp>

glfw::window::window(int width, int height, std::string title) :
        title(title) ,raw_handle(std::shared_ptr<GLFWwindow>(glfwCreateWindow(width,height,title.c_str(), nullptr,
                                                                               nullptr),glfwDestroyWindow)){
}

GLFWwindow *glfw::window::data() {
    return raw_handle.get();
}

void glfw::window::show() {
    glfwShowWindow(raw_handle.get());
}

void glfw::window::hide() {
    if (glfwGetWindowAttrib(raw_handle.get(), GLFW_VISIBLE))
        glfwHideWindow(raw_handle.get());
}

void glfw::window::update() {

    // window destroy event received
    if (glfwWindowShouldClose(raw_handle.get())) {
        delete this;
    } else {
        glfwSwapBuffers(raw_handle.get());
    }
}

const std::string glfw::window::get_title() noexcept {
    return title;
}

void glfw::window::get_focus() {
    glfwFocusWindow(raw_handle.get());
}

const bool glfw::window::is_show() {
    return static_cast<const bool>(glfwGetWindowAttrib(raw_handle.get(), GLFW_VISIBLE));
}


std::map<glfw::window_id_t, glfw::window_ptr_t> glfw::window_manager::m_raw_handle_pool;
std::map<glfw::window_id_t, std::function<void(glfw::window_ptr_t)>> glfw::window_manager::close_callback_map = {};
std::map<glfw::window_id_t, std::function<void(glfw::window_ptr_t, int, int, int,
                                               int)>> glfw::window_manager::keyboard_callback_map = {};

glfw::window_manager::window_manager() {
}

glfw::window_manager::~window_manager() {
}

glfw::window_id_t glfw::window_manager::create_window(int width, int height, std::string title,
                                                      const std::function<void(window_ptr_t)> &callback) {
    glfw::window_id_t id = get_pool().size();
    id.set_manager(this);
    get_pool()[id] = std::make_shared<glfw::window>(width, height, title);
    glfwSetWindowCloseCallback(id.get_window().get()->data(), invoke_close_callback);
    glfwSetKeyCallback(id.get_window().get()->data(), invoke_key_callback);
    if (callback != nullptr) {
        close_callback_map[id] = callback;
    }
    return id;
}

std::map<glfw::window_id_t, glfw::window_ptr_t> &glfw::window_manager::get_pool() noexcept {
    return glfw::window_manager::m_raw_handle_pool;
}

glfw::window_ptr_t &glfw::window_manager::get_window(glfw::window_id_t id) {
    return m_raw_handle_pool.at(id);
}

void glfw::window_manager::invoke_key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) {
    auto id = find_id(handle);
    if (id.m_id != -1) {
        if (keyboard_callback_map[id]) {
            keyboard_callback_map[id](id.get_window(), key, scancode, action, mods);
        }
    }
}

void glfw::window_manager::invoke_close_callback(GLFWwindow *handle) {
    auto id = find_id(handle);
    if (id.m_id != -1) {
        if (close_callback_map[id]) {
            close_callback_map[id](id.get_window());
        }
        m_raw_handle_pool.erase(id);
    }
}

glfw::window_id_t glfw::window_manager::find_id(GLFWwindow *handle) {
    for (auto &x: m_raw_handle_pool) {
        if (x.second->data() == handle) {
            return x.first;
        }
    }
    throw std::runtime_error("No such handle!");
}

const bool glfw::window_manager::all_cleared() {
    return m_raw_handle_pool.size() == 0;
}

void glfw::window_manager::update_all() {
    for (auto &x : m_raw_handle_pool) {
        x.second->update();
    }
}

void glfw::window_manager::add_keyboard_callback(glfw::window_id_t id,
                                                 const std::function<void(window_ptr_t, int, int, int,
                                                                          int)> &callback) {
    if (m_raw_handle_pool.find(id) !=
        m_raw_handle_pool.end()) // If specified window does not have a callback, simply assigned
    {
        if (keyboard_callback_map.find(id) == keyboard_callback_map.end()) {
            keyboard_callback_map[id] = callback;
        } else // if it has, append it
        {
            keyboard_callback_map[id] = [old_wrapper{keyboard_callback_map[id]}, this, callback](window_ptr_t ptr,
                                                                                                 int key, int scancode,
                                                                                                 int action, int mods) {
                old_wrapper(ptr, key, scancode, action, mods);
                callback(ptr, key, scancode, action, mods);
            };
        }
    } else {
        throw std::runtime_error("window not exist!");
    }
}

void
glfw::window_manager::add_window_close_callback(glfw::window_id_t id, const std::function<void(window_ptr_t)> &callback) {
    if (m_raw_handle_pool.find(id) !=
        m_raw_handle_pool.end()) // If specified window does not have a callback, simply assigned
    {
        if (close_callback_map.find(id) == close_callback_map.end()) {
            close_callback_map[id] = callback;
        } else // if it has, append it
        {
            close_callback_map[id] = [old_wrapper{close_callback_map[id]}, this, callback](window_ptr_t ptr) {
                old_wrapper(ptr);
                callback(ptr);
            };
        }
    } else {
        throw std::runtime_error("window not exist!");
    }
}

glfw::window_ptr_t &glfw::window_manager::get_window(int id) {
    window_id_t id_wrapper(id);
    id_wrapper.set_manager(this);
    return get_window(id_wrapper);
}

void glfw::window_manager::show_all() {
    for (auto &x : get_pool()) {
        x.second->show();
    }
}

void glfw::window_manager::set_use_vulkan() {
    if (!glfwVulkanSupported())  // did not gain a vulkan support
    {
        throw std::runtime_error("GLFW Vulkan not supported!");
    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // needed
    }
}


glfw::window_id_t::window_id_t(int id) : m_id{id}, m_manager{nullptr} {

}

void glfw::window_id_t::set_id(int id) {
    m_id = id;
}

void glfw::window_id_t::set_manager(glfw::window_manager *manager) {
    m_manager = manager;
}

void glfw::window_id_t::set_manager(glfw::window_manager &manager) {
    m_manager = &manager;
}


glfw::window_ptr_t &glfw::window_id_t::get_window() {
    if (m_manager)
        return m_manager->get_window(*this);
    else  // pointer to manager is null, maybe this is a manually inited var
    {
        std::stringstream ss;
        ss << this << " id:" << m_id << " " << __FUNCTION__ << " : m_manager is null";
        throw std::runtime_error(ss.str());
    }

}
