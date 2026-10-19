#pragma once

#include "vulkan/raw.hh"

namespace fl::pVulkan::Managed {

struct LogicalDevice {
  struct Deleter {
    using pointer = VkDevice;

    RAW::Allocator allocator = nullptr;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    RAW::Allocator allocator = nullptr;
    VkPhysicalDevice physical_device;
    std::vector<uint32_t> family_indices;
    std::vector<const char*> extensions;

    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_device(VkPhysicalDevice device) &&;
    Config&& set_family(std::vector<uint32_t> families) &&;
    Config&& set_extensions(std::vector<const char*> exts) &&;
  };

  [[nodiscard]]
  static
  Pointer create(const Config& config);

  [[nodiscard]]
  static
  Pointer create(VkPhysicalDevice physical_device,
                 RAW::Allocator allocator,
                 const std::vector<uint32_t>& family_indices,
                 const std::vector<const char*>& extensions);

  [[nodiscard]]
  static
  Pointer create(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* const device_create_info, RAW::Allocator allocator);
};

}
