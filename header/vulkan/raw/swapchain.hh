#pragma once

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

namespace flp::Vulkan::RAW {

VkSwapchainKHR
create_swapchain(
  VkDevice device,
  VkSurfaceKHR surface,
  Allocator allocator,
  uint32_t image_count,
  VkFormat format,
  VkColorSpaceKHR color_space,
  VkExtent2D extent,
  VkSurfaceTransformFlagBitsKHR pretransform,
  VkPresentModeKHR present_mode,
  const std::vector<uint32_t>& queue_indices);

void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain, Allocator allocator);

}
