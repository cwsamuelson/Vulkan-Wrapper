#pragma once

#include <vulkan/vulkan_core.h>

// #include semver

#include <optional>
#include <string>
#include <vector>

namespace flp::Vulkan::RAW {

// these should all be specified together
struct DebugMessengerData {
  VkDebugUtilsMessengerCreateInfoEXT* debug_messenger_create_info;
  PFN_vkDebugUtilsMessengerCallbackEXT debug_callback;
  void* user_data;

  DebugMessengerData(VkDebugUtilsMessengerCreateInfoEXT* create_info, PFN_vkDebugUtilsMessengerCallbackEXT callback, void* data);
};

// === Instance

VkInstance create_instance(const std::string& app_name, const semver::version app_version, const std::vector<const char*>& extension_names, const std::vector<const char*>& validation_layer_names, Allocator pallocator, std::optional<DebugMessengerData> debug_messenger_data);

VkInstance create_instance(const std::string& app_name, semver::version app_version, const std::vector<std::string>& extension_names, const std::vector<std::string>& validation_layer_names, Allocator pallocator);

VkInstance create_instance(const std::string& app_name, semver::version app_version, const std::vector<std::string>& extension_names, const std::vector<std::string>& validation_layer_names);

void destroy_instance(VkInstance instance, Allocator pallocator);

}
