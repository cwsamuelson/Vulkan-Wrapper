#pragma once

#include "vulkan/raw.hh"

namespace flp::Vulkan::Managed {

struct DebugMessenger {
  struct Deleter {
    using pointer = VkDebugUtilsMessengerEXT;

    VkInstance instance = nullptr;
    RAW::Allocator allocator = nullptr;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    const Instance::Pointer& instance;
    const VkDebugUtilsMessengerCreateInfoEXT* create_info;
    RAW::Allocator allocator;

    //Config&& set_instance(Instance::Pointer) &&;
    Config&& set_create_info(const VkDebugUtilsMessengerCreateInfoEXT*) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
  };

  [[nodiscard]]
  static
  Pointer create(const Config& config);

  [[nodiscard]]
  static
  Pointer create(const Instance::Pointer& instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, RAW::Allocator allocator);
};

}
