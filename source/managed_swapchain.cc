#include "vulkan/managed/swapchain.hh"

namespace flp::Vulkan::Managed {

void Swapchain::Deleter::operator()(Vulkan::Swapchain::Deleter::pointer p) {
  RAW::destroy_swapchain(device, p, allocator);
}

auto Swapchain::Config::set_device(VkDevice dev)&& -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Swapchain::Config::set_surface(VkSurfaceKHR srfc) && -> Config&& {
  surface = srfc;
  return std::move(*this);
}

auto Swapchain::Config::set_allocator(RAW::Allocator alc) && -> Config&& {
  allocator = alc;
  return std::move(*this);
}

auto Swapchain::Config::set_image_count(uint32_t count) && -> Config&& {
  image_count = count;
  return std::move(*this);
}

auto Swapchain::Config::set_format(VkFormat fmt) && -> Config&& {
  format = fmt;
  return std::move(*this);
}

auto Swapchain::Config::set_color_space(VkColorSpaceKHR space) && -> Config&& {
  color_space = space;
  return std::move(*this);
}

auto Swapchain::Config::set_extent(VkExtent2D ext) && -> Config&& {
  extent = ext;
  return std::move(*this);
}

auto Swapchain::Config::set_pretransform(VkSurfaceTransformFlagBitsKHR transform) && -> Config&& {
  pretransform = transform;
  return std::move(*this);
}

auto Swapchain::Config::set_present_mode(VkPresentModeKHR mode) && -> Config&& {
  present_mode = mode;
  return std::move(*this);
}

auto Swapchain::Config::set_queue_indices(std::vector<uint32_t> indices) && -> Config&& {
  queue_indices = std::move(indices);
  return std::move(*this);
}

auto Swapchain::create(VkDevice device, VkSurfaceKHR surface, RAW::Allocator allocator, uint32_t image_count,
                       VkFormat format, VkColorSpaceKHR color_space, VkExtent2D extent,
                       VkSurfaceTransformFlagBitsKHR pretransform, VkPresentModeKHR present_mode,
                       const std::vector<uint32_t>& queue_indices) -> Pointer {
  auto* swapchain = RAW::create_swapchain(device, surface, allocator, image_count, format, color_space, extent,
                                         pretransform, present_mode, queue_indices);

  return {swapchain,
          {
            .device = device,
            .allocator = allocator,
          }};
}

auto Swapchain::create(const Vulkan::Swapchain::Config& config) -> Pointer {
  return create(config.device, config.surface, config.allocator, config.image_count, config.format,
                config.color_space, config.extent, config.pretransform, config.present_mode, config.queue_indices);
}

}
