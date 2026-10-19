#include "vulkan/managed/logical-device.hh"

namespace flp::Vulkan::Managed {

void LogicalDevice::Deleter::operator()(Vulkan::LogicalDevice::Deleter::pointer p) {
  RAW::destroy_device(p, allocator);
}

auto LogicalDevice::Config::set_allocator(RAW::Allocator alloc)&& -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto LogicalDevice::Config::set_device(VkPhysicalDevice device) && -> Config&& {
  physical_device = device;
  return std::move(*this);
}

auto LogicalDevice::Config::set_family(std::vector<uint32_t> families) && -> Config&& {
  family_indices = families;
  return std::move(*this);
}

auto LogicalDevice::Config::set_extensions(std::vector<const char*> exts) && -> Config&& {
  extensions = exts;
  return std::move(*this);
}

auto LogicalDevice::create(const Vulkan::LogicalDevice::Config& config) -> Pointer {
  return create(config.physical_device, config.allocator, config.family_indices, config.extensions);
}

auto LogicalDevice::create(VkPhysicalDevice physical_device,
                           RAW::Allocator allocator,
                           const std::vector<uint32_t>& family_indices,
                           const std::vector<const char*>& extensions) -> Pointer {
  const float priority = 1.0f;

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

  std::ranges::transform(family_indices, std::back_inserter(queue_create_infos), [&priority](const uint32_t index) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &priority;

    return queue_create_info;
  });

  VkPhysicalDeviceFeatures features{};

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.queueCreateInfoCount = queue_create_infos.size();
  device_create_info.pEnabledFeatures = &features;
  device_create_info.ppEnabledExtensionNames = extensions.data();
  device_create_info.enabledExtensionCount = extensions.size();

  return create(physical_device, &device_create_info, allocator);
}

auto LogicalDevice::create(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* const device_create_info,
                              RAW::Allocator allocator) -> Pointer {
  auto* device = RAW::create_device(physical_device, device_create_info, allocator);

  return {device, {
      .allocator = allocator
  }};
}

}
