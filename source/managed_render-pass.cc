#include "vulkan/managed/render-pass.hh"

namespace flp::Vulkan::Managed {

void RenderPass::Deleter::operator()(pointer p) {
  RAW::destroy_render_pass(device, allocator, p);
}

auto RenderPass::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto RenderPass::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto RenderPass::Config::set_attachments(std::vector<VkAttachmentDescription> attach) && -> Config&& {
  attachments = std::move(attach);
  return std::move(*this);
}

auto RenderPass::Config::set_sub_passes(std::vector<VkSubpassDescription> passes) && -> Config&& {
  sub_passes = std::move(passes);
  return std::move(*this);
}

auto RenderPass::Config::set_dependencies(std::vector<VkSubpassDependency> deps) && -> Config&& {
  dependencies = std::move(deps);
  return std::move(*this);
}

auto RenderPass::create(VkDevice device,
                        RAW::Allocator allocator,
                        const std::vector<VkAttachmentDescription>& attachments,
                        const std::vector<VkSubpassDescription>& sub_passes,
                        const std::vector<VkSubpassDependency>& dependencies) -> Pointer {
  auto* render_pass = RAW::create_render_pass(device, allocator, attachments, sub_passes, dependencies);

  return {
    render_pass,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto RenderPass::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.attachments, config.sub_passes, config.dependencies);
}

}
