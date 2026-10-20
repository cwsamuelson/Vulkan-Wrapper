#pragma once

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace flp::Vulkan::Managed {

struct Swapchain {
  struct Deleter {
    using pointer = VkSwapchainKHR;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    VkSurfaceKHR surface;
    RAW::Allocator allocator;
    uint32_t image_count;
    VkFormat format;
    VkColorSpaceKHR color_space;
    VkExtent2D extent;
    VkSurfaceTransformFlagBitsKHR pretransform;
    VkPresentModeKHR present_mode;
    std::vector<uint32_t> queue_indices;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_surface(VkSurfaceKHR srfc) &&;
    Config&& set_allocator(RAW::Allocator alc) &&;
    Config&& set_image_count(uint32_t count) &&;
    Config&& set_format(VkFormat fmt) &&;
    Config&& set_color_space(VkColorSpaceKHR space) &&;
    Config&& set_extent(VkExtent2D ext) &&;
    Config&& set_pretransform(VkSurfaceTransformFlagBitsKHR transform) &&;
    Config&& set_present_mode(VkPresentModeKHR mode) &&;
    Config&& set_queue_indices(std::vector<uint32_t> indices) &&;
  };

  [[nodiscard]]
  static
  Pointer create(
    VkDevice device,
    VkSurfaceKHR surface,
    RAW::Allocator allocator,
    uint32_t image_count,
    VkFormat format,
    VkColorSpaceKHR color_space,
    VkExtent2D extent,
    VkSurfaceTransformFlagBitsKHR pretransform,
    VkPresentModeKHR present_mode,
    const std::vector<uint32_t>& queue_indices);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
