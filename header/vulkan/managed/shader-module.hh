#pragma once

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

namespace flp::Vulkan::Managed {

struct ShaderModule {
  struct Deleter {
    using pointer = VkShaderModule;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    std::vector<char> shader_code;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_shader_code(std::vector<char> code) &&;
  };

  [[nodiscard]]
  static
  Pointer create(VkDevice device, RAW::Allocator allocator, const std::vector<char>& shader_code);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
