#include "vulkan/managed/pipeline-layout.hh"

namespace flp::Vulkan::Managed {

void PipelineLayout::Deleter::operator()(pointer p) {
  RAW::destroy_pipeline_layout(device, allocator, p);
}

auto PipelineLayout::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto PipelineLayout::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto PipelineLayout::create(VkDevice device, RAW::Allocator allocator) -> Pointer {
  auto* layout = RAW::create_pipeline_layout(device, allocator);

  return {
    layout,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto PipelineLayout::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator);
}

}
