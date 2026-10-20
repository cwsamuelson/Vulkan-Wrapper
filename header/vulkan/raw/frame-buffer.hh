#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace flp::Vulkan::RAW {

VkFramebuffer create_frame_buffer(
  VkDevice device,
  RAW::Allocator allocator,
  VkRenderPass render_pass,
  const std::vector<VkImageView>& attachments,
  VkExtent2D extents
);

void destroy_frame_buffer(VkDevice device, RAW::Allocator allocator, VkFramebuffer frame_buffer);

}
