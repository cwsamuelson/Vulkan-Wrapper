#pragma once

namespace flp::Vulkan::raw {

VkSemaphore create_semaphore(VkDevice device, RAW::Allocator allocator);

void destroy_semaphore(VkDevice device, RAW::Allocator allocator, VkSemaphore semaphore);

}
