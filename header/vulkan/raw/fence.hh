#pragma once

namespace flp::Vulkan::raw {

VkFence create_fence(VkDevice device, RAW::Allocator allocator, VkFlags flags);

void destroy_fence(VkDevice device, RAW::Allocator allocator, VkFence fence);

}
