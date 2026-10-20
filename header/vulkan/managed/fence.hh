#pragma once

#include <vulkan/vulkan_core.h>

#include <memory>

namespace flp::Vulkan::Managed {

struct Fence {
  struct Deleter {
    using pointer = VkFence;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    VkFlags flags;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_flags(VkFlags flgs) &&;
  };

  [[nodiscard]]
  static
  Pointer create(VkDevice device, RAW::Allocator allocator, VkFlags flags);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
