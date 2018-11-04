//
// Created by sleeplessy on 11/3/18.
//

#ifndef GLFWXX_INSTANCE_MANAGER_HPP
#define GLFWXX_INSTANCE_MANAGER_HPP

// make this header independent with others
#include <glfwxx/common.hpp>
#include <glfwxx/window.hpp>
#include <future>
#include <vulkan/vulkan.hpp>

namespace glfw {
    enum GLFW_INSTANCE_FLAG {
        GLFW_RUNNING,   // instance is running
        GLFW_WANT_QUIT, // instance want quit
    };

    enum GLFW_POLLING_FLAG {
        GLFW_USE_POLL,  // use glfwPollEvents
        GLFW_USE_WAIT,  // use glfwWaitEvents
    };

    struct vulkan_info {
        vulkan_info();

        std::vector<vk::PhysicalDevice> enumerate_physical_devices();
        std::vector<vk::QueueFamilyProperties> get_queue_family_properties(std::size_t index = 0);
        vk::UniqueDevice create_device(vk::DeviceCreateInfo create_info, std::size_t index);
        uint32_t ext_count;  // the count of extention
        vk::InstanceCreateInfo  instance_create_info;
        vk::UniqueInstance instance;
        std::vector<const char *> ext_lists;    // list of checked extention
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

        vulkan_info& get_vk_info() noexcept;

        vk::InstanceCreateInfo &create_vk_instance_create_info(vk::InstanceCreateFlags flags = {},
                                                               const vk::ApplicationInfo *ptr_application_info = nullptr, uint32_t layers_count = 0,const char* enabled_layer_name[] = nullptr);
        vk::Instance& create_vk_instace(vk::InstanceCreateInfo& create_info);

        const bool check_layer_validation(std::string layer_name);
        vk::UniqueSurfaceKHR create_window_surface(vk::Instance& vk_instance, window_ptr_t& window_ptr);
        void poll();

    private:
        window_manager m_window_manager;
        GLFW_POLLING_FLAG m_event_flag;

        std::promise<GLFW_INSTANCE_FLAG> m_instance_flag;
        std::future<glfw::GLFW_INSTANCE_FLAG> m_quiting_flags;
        vulkan_info m_vk_info;
    };
}

#endif //GLFWXX_INSTANCE_MANAGER_HPP
