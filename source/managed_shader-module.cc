#include "vulkan/managed/shader-module.hh"

namespace flp::Vulkan::Managed {

void ShaderModule::Deleter::operator()(Vulkan::ShaderModule::Deleter::pointer p) {
  RAW::destroy_shader_module(device, allocator, p);
}

auto ShaderModule::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto ShaderModule::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto ShaderModule::Config::set_shader_code(std::vector<char> code) && -> Config&& {
  shader_code = std::move(code);
  return std::move(*this);
}

auto ShaderModule::create(VkDevice device, RAW::Allocator allocator, const std::vector<char>& shader_code) -> Pointer {
  auto* shader_module = RAW::create_shader_module(device, allocator, shader_code);

  return {
    shader_module,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto ShaderModule::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.shader_code);
}

}
