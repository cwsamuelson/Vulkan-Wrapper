#pragma once

namespace flp::Vulkan::raw {

VkDebugUtilsMessengerEXT create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, Allocator pallocator);

VkDebugUtilsMessengerEXT create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info);

void destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, Allocator allocator);

}
