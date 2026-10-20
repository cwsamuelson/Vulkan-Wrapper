#pragma once

#include <vulkan/vulkan_core.h>

#include <memory>

namespace flp::Vulkan::Managed {

struct PipelineLayout {
  struct Deleter {
    using pointer = VkPipelineLayout;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
  };

  [[nodiscard]]
  static
  Pointer create(VkDevice device, RAW::Allocator allocator);

  [[nodiscard]]
  static
  Pointer create(const Config&);
};

}
