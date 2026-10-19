#pragma once

namespace flp::Vulkan::Managed {

struct FrameBuffer {
  struct Deleter {
    using pointer = VkFramebuffer;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    VkRenderPass render_pass;
    std::vector<VkImageView> attachments;
    VkExtent2D extents;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_render_pass(VkRenderPass pass) &&;
    Config&& set_attachments(std::vector<VkImageView> views) &&;
    Config&& set_extent(VkExtent2D ext) &&;
  };

  [[nodiscard]]
  static
  Pointer create(VkDevice device, RAW::Allocator allocator, VkRenderPass render_pass,
                 const std::vector<VkImageView>& attachments, VkExtent2D extents);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
