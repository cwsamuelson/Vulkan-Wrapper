#pragma once

namespace flp::Vulkan::raw {

VkImageView create_image_view(VkDevice device, RAW::Allocator allocator, VkImage image, VkFormat format);

void destroy_image_view(VkDevice device, VkImageView image_view, RAW::Allocator);

}
