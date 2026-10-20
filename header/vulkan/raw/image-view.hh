#pragma once

#include <vulkan/vulkan_core.h>

namespace flp::Vulkan::RAW {

VkImageView create_image_view(VkDevice device, RAW::Allocator allocator, VkImage image, VkFormat format);

void destroy_image_view(VkDevice device, VkImageView image_view, RAW::Allocator);

}
