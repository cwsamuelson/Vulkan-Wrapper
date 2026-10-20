#pragma once

#include "vulkan/raw/allocator.hh"

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <memory>

namespace flp::Vulkan::Managed {

struct CommandPool {
  struct Deleter {
    using pointer = VkCommandPool;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    uint32_t queue_family_index;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_queue_family_index(uint32_t index) &&;
  };

  [[nodiscard]]
  static
  Pointer create(VkDevice device, RAW::Allocator allocator, uint32_t queue_family_index);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
