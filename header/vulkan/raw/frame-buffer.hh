#pragma once

namespace flp::Vulkan::raw {

VkFramebuffer create_frame_buffer(
  VkDevice device,
  RAW::Allocator allocator,
  VkRenderPass render_pass,
  const std::vector<VkImageView>& attachments,
  VkExtent2D extents
);

void destroy_frame_buffer(VkDevice device, RAW::Allocator allocator, VkFramebuffer frame_buffer);

}
