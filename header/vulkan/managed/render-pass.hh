#pragma once

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

namespace flp::Vulkan::Managed {

struct RenderPass {
  struct Deleter {
    using pointer = VkRenderPass;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    std::vector<VkAttachmentDescription> attachments;
    std::vector<VkSubpassDescription> sub_passes;
    std::vector<VkSubpassDependency> dependencies;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_attachments(std::vector<VkAttachmentDescription> attach) &&;
    Config&& set_sub_passes(std::vector<VkSubpassDescription> passes) &&;
    Config&& set_dependencies(std::vector<VkSubpassDependency> deps) &&;
  };

  [[nodiscard]]
  static
  Pointer create(
    VkDevice device,
    RAW::Allocator allocator,
    const std::vector<VkAttachmentDescription>& attachments,
    const std::vector<VkSubpassDescription>& sub_passes,
    const std::vector<VkSubpassDependency>& dependencies);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
