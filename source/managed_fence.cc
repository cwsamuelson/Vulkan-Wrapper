#include "vulkan/managed/fence.hh"

namespace flp::Vulkan::Managed {

void Fence::Deleter::operator()(pointer p) {
  RAW::destroy_fence(device, allocator, p);
}

auto Fence::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Fence::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto Fence::Config::set_flags(VkFlags flgs) && -> Config&& {
  flags = flgs;
  return std::move(*this);
}

auto Fence::create(VkDevice device, RAW::Allocator allocator, VkFlags flags) -> Pointer {
  auto* fence = RAW::create_fence(device, allocator, flags);

  return {
    fence,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto Fence::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.flags);
}

}
