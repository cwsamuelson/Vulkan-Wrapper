#pragma once

#include <vulkan/vulkan_core.h>

// #include semver

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace flp::Vulkan::Managed {

struct Instance {
  struct Deleter {
    using pointer = VkInstance;

    RAW::Allocator allocator = nullptr;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    std::string app_name;
    semver::version app_version;
    std::vector<const char*> extension_names = default_extensions;
    std::vector<const char*> validation_layer_names = default_validation_layers;
    RAW::Allocator allocator = nullptr;
    std::optional<RAW::DebugMessengerData> debug_messenger;

    static const inline std::vector<const char*> default_extensions;
    static const inline std::vector<const char*> default_validation_layers {
      "VK_LAYER_KHRONOS_validation",
    };

    Config&& set_app_name(std::string_view name) &&;
    Config&& set_version(semver::version version) &&;
    Config&& set_extension_names(std::vector<const char*> names) &&;
    Config&& set_validation_layer_names(std::vector<const char*> names) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_debug_messenger(std::optional<RAW::DebugMessengerData> messenger) &&;
  };

  [[nodiscard]]
  static
  Pointer create(const Config& config);

  [[nodiscard]]
  static
  Pointer create(const std::string& app_name, semver::version app_version, const std::vector<const char*>& extension_names, const std::vector<const char*>& validation_layer_names, RAW::Allocator pallocator, std::optional<RAW::DebugMessengerData> debug_messenger);
};

}
