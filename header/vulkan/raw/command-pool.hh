#pragma once

namespace flp::Vulkan::raw {

VkCommandPool create_command_pool(VkDevice device, RAW::Allocator allocator, uint32_t queue_family_index);

void destroy_command_pool(VkDevice device, RAW::Allocator allocator, VkCommandPool command_pool);

}
