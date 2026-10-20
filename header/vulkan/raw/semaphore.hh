#pragma once

#include <vulkan/vulkan_core.h>

namespace flp::Vulkan::RAW {

VkSemaphore create_semaphore(VkDevice device, RAW::Allocator allocator);

void destroy_semaphore(VkDevice device, RAW::Allocator allocator, VkSemaphore semaphore);

}
