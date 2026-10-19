#include "vulkan/managed/semaphore.hh"

namespace flp::Vulkan::Managed {

void Semaphore::Deleter::operator()(pointer p) {
  RAW::destroy_semaphore(device, allocator, p);
}

auto Semaphore::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Semaphore::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto Semaphore::create(VkDevice device, RAW::Allocator allocator) -> Pointer {
  auto* semaphore = RAW::create_semaphore(device, allocator);

  return {
    semaphore,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto Semaphore::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator);
}

}
