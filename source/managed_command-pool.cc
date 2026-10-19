#include "vulkan/managed/command-pool.hh"

namespace flp::Vulkan::Managed {

void CommandPool::Deleter::operator()(pointer p) {
  RAW::destroy_command_pool(device, allocator, p);
}

auto CommandPool::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto CommandPool::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto CommandPool::Config::set_queue_family_index(const uint32_t index) && -> Config&& {
  queue_family_index = index;
  return std::move(*this);
}

auto CommandPool::create(VkDevice device, RAW::Allocator allocator, const uint32_t queue_family_index) -> Pointer {
  auto* command_pool = RAW::create_command_pool(device, allocator, queue_family_index);

  return {
    command_pool,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto CommandPool::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.queue_family_index);
}

}
