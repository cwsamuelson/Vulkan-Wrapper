#include "vulkan/managed/debug-messenger.hh"

namespace flp::Vulkan::Managed {

void DebugMessenger::Deleter::operator()(Vulkan::DebugMessenger::Deleter::pointer p) {
  RAW::destroy_debug_messenger(instance, p, allocator);
}

auto DebugMessenger::Config::set_create_info(const VkDebugUtilsMessengerCreateInfoEXT* info) && -> Config&& {
  create_info = info;
  return std::move(*this);
}

auto DebugMessenger::Config::set_allocator(RAW::Allocator alloc) && -> Config&&{
  allocator = alloc;
  return std::move(*this);
}

auto DebugMessenger::create(const Vulkan::DebugMessenger::Config& config) -> Pointer {
  return create(config.instance, config.create_info, config.allocator);
}

auto DebugMessenger::create(const Instance::Pointer& instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info,
                               RAW::Allocator allocator) -> Pointer {
  auto* debug_messenger = RAW::create_debug_messenger(instance.get(), create_info, allocator);
  return {debug_messenger, {
      .instance = instance.get(),
      .allocator = allocator,
  }};
}

}
