#pragma once

namespace flp::Vulkan::raw {

VkDevice create_device(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* create_info, Allocator allocator);

void destroy_device(VkDevice device, Allocator allocator);

}
