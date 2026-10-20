#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace flp::Vulkan::RAW {

VkRenderPass create_render_pass(
  VkDevice device,
  RAW::Allocator allocator,
  const std::vector<VkAttachmentDescription>& attachments,
  const std::vector<VkSubpassDescription>& sub_passes,
  const std::vector<VkSubpassDependency>& dependencies
);

void destroy_render_pass(VkDevice device, RAW::Allocator allocator, VkRenderPass render_pass);

}
