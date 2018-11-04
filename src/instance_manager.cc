#include <stdexcept>
#include <thread>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glfwxx/instance_manager.hpp>
#include <iostream>

glfw::instance_manager::instance_manager() :
        m_event_flag{glfw::GLFW_POLLING_FLAG::GLFW_USE_POLL},
        m_instance_flag{},
        m_quiting_flags{m_instance_flag.get_future()} {
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

void glfw::instance_manager::set_vulkan_flag(const bool use_opengl) {
    glfwDefaultWindowHints();
    if (!use_opengl) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // if one don't need opengl...
    }
}

vk::InstanceCreateInfo &glfw::instance_manager::create_vk_instance_create_info(vk::InstanceCreateFlags flags,
                                                                               const vk::ApplicationInfo *ptr_application_info,
                                                                               uint32_t layers_count,
                                                                               const char **enabled_layer_name) {
    auto ext_list = glfwGetRequiredInstanceExtensions(&m_vk_info.ext_count);  // get the ext count
    m_vk_info.ext_lists = std::vector<const char*>(ext_list,ext_list+m_vk_info.ext_count);
    std::vector<std::string> layer_name_vec(enabled_layer_name,enabled_layer_name+layers_count);
    for(auto &x: layer_name_vec)
    {
        if(!check_layer_validation(x))  // if there is an invalid layer
        {
            std::string msg = "Invalid layer ";
            msg+= x;
            throw std::runtime_error(msg);
        }
    }

    m_vk_info.instance_create_info = vk::InstanceCreateInfo(flags,ptr_application_info,layers_count,enabled_layer_name,m_vk_info.ext_count,m_vk_info.ext_lists.data());
    return m_vk_info.instance_create_info;
}

vk::Instance &glfw::instance_manager::create_vk_instace(vk::InstanceCreateInfo &create_info) {
    m_vk_info.instance = vk::createInstanceUnique(create_info);
    return m_vk_info.instance.get();
}

vk::UniqueSurfaceKHR
glfw::instance_manager::create_window_surface(vk::Instance &vk_instance, glfw::window_ptr_t &window_ptr) {
    vk::SurfaceKHR surface{};
    auto surface_ptr = VkSurfaceKHR(surface);
    if(glfwCreateWindowSurface(VkInstance(vk_instance),window_ptr->data(), nullptr,&surface_ptr) != VkResult::VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create surface");
    }
    return vk::UniqueSurfaceKHR(surface_ptr,m_vk_info.instance.get());
}

glfw::vulkan_info &glfw::instance_manager::get_vk_info() noexcept {
    return m_vk_info;
}

const bool glfw::instance_manager::check_layer_validation(std::string layer_name) {
    uint32_t count;
    vk::enumerateInstanceLayerProperties(&count,nullptr);
    std::vector<vk::LayerProperties> available_layers(count);
    vk::enumerateInstanceLayerProperties(&count,available_layers.data());
    for(auto &x: available_layers)
    {
        if(layer_name == x.description)
            return true;
    }
    return false;
}



glfw::vulkan_info::vulkan_info() : ext_count{0}, ext_lists{} {

}

std::vector<vk::PhysicalDevice> glfw::vulkan_info::enumerate_physical_devices() {
    return instance->enumeratePhysicalDevices();
}

std::vector<vk::QueueFamilyProperties> glfw::vulkan_info::get_queue_family_properties(std::size_t index) {
    return enumerate_physical_devices().at(index).getQueueFamilyProperties();
}

vk::UniqueDevice glfw::vulkan_info::create_device(vk::DeviceCreateInfo create_info, std::size_t index) {
    return enumerate_physical_devices().at(index).createDeviceUnique(create_info);
}
