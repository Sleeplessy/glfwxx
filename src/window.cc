//
// Created by sleeplessy on 11/3/18.
//

#include <algorithm>

#include <glfwxx/common.hpp>
#include <glfwxx/window.hpp>
#include <GLFW/glfw3.h>
#include <cassert>

glfw::window::window(int width, int height, std::string title) : raw_handle{
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)}, title(title) {

}

GLFWwindow *glfw::window::data() {
    return raw_handle;
}

glfw::window::~window() {
    glfwDestroyWindow(raw_handle);
}


void glfw::window::show() {
    glfwShowWindow(data());
}

void glfw::window::update() {

    // window destroy event received
    if (glfwWindowShouldClose(raw_handle)) {
        delete this;
    } else {

        glfwSwapBuffers(raw_handle);
    }
}

const std::string glfw::window::get_title() noexcept {
    return title;
}

void glfw::window::get_focus() {
    glfwFocusWindow(raw_handle);
}


std::map<glfw::window_id_t, glfw::window_ptr_t> glfw::window_manager::m_raw_handle_pool;
std::map<glfw::window_id_t, std::function<void(glfw::window_ptr_t)>> glfw::window_manager::close_callback_map = {};

glfw::window_manager::window_manager() {
}

glfw::window_manager::~window_manager() {
}

glfw::window_id_t glfw::window_manager::create_window(int width, int height, std::string title,
                                                      const std::function<void(window_ptr_t)> &callback) {
    glfw::window_id_t id = get_pool().size();
    get_pool()[id] = std::make_shared<glfw::window>(width, height, title);
    id.set_manager(*this);
    glfwSetWindowCloseCallback(id.get_window().get()->data(), invoke_close_callback);
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

void glfw::window_manager::invoke_close_callback(GLFWwindow *handle) {
    auto id = find_id(handle);
    if (id.m_id != -1) {
        close_callback_map[id](id.get_window());
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

void glfw::window_manager::add_window_callback(glfw::window_id_t id, const std::function<void(window_ptr_t)> &callback) {
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
    id_wrapper.set_manager(*this);
    return get_window(id_wrapper);
}


glfw::window_id_t::window_id_t(int id) : m_id(id) {

}

void glfw::window_id_t::set_id(int id) {
    m_id = id;
}

void glfw::window_id_t::set_manager(glfw::window_manager &manager) {
    m_manager = &manager;
}

glfw::window_ptr_t &glfw::window_id_t::get_window() {
    return m_manager->get_window(*this);
}
