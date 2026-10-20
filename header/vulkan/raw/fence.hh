#pragma once

#include <vulkan/vulkan_core.h>

namespace flp::Vulkan::RAW {

VkFence create_fence(VkDevice device, RAW::Allocator allocator, VkFlags flags);

void destroy_fence(VkDevice device, RAW::Allocator allocator, VkFence fence);

}
