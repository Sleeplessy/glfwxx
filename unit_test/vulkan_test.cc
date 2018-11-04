//
// Created by sleeplessy on 11/4/18.
//

#include <glfwxx/instance_manager.hpp>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>
int main()
{
    glfw::instance_manager manager;
    manager.set_vulkan_flag();
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_LUNARG_standard_validation"
    };
    auto vk_create_info = manager.create_vk_instance_create_info(vk::InstanceCreateFlagBits{}, nullptr);
    auto vk_instance = manager.create_vk_instace(vk_create_info);
    auto physicalDevices = manager.get_vk_info().enumerate_physical_devices();
    std::cout << physicalDevices.size() << " device(s) found" << std::endl;
    for(auto & x: physicalDevices)
    {
        std::cout << x.getProperties().deviceName << std::endl;
    }

    assert(!physicalDevices.empty());

    auto queueFamilyPropertices =  manager.get_vk_info().get_queue_family_properties();
    long graphicsQueueFamilyIndex = std::distance(queueFamilyPropertices.begin(),
                                                    std::find_if(queueFamilyPropertices.begin(),
                                                                 queueFamilyPropertices.end(),
                                                                 [](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; }));
    assert(graphicsQueueFamilyIndex < queueFamilyPropertices.size());
    float queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), static_cast<uint32_t>(graphicsQueueFamilyIndex), 1, &queuePriority);
    vk::UniqueDevice device = physicalDevices[0].createDeviceUnique(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), 1, &deviceQueueCreateInfo));
    auto surface = manager.create_window_surface(manager.get_vk_info().instance.get(),manager.create_window(400,400).get_window());
    manager.poll();
    return 0;
}