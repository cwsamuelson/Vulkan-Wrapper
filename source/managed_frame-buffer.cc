#include "vulkan/managed/frame-buffer.hh"

namespace flp::Vulkan::Managed {

void FrameBuffer::Deleter::operator()(pointer p) {
  RAW::destroy_frame_buffer(device, allocator, p);
}

auto FrameBuffer::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto FrameBuffer::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto FrameBuffer::Config::set_render_pass(VkRenderPass pass) && -> Config&& {
  render_pass = pass;
  return std::move(*this);
}

auto FrameBuffer::Config::set_attachments(std::vector<VkImageView> views) && -> Config&& {
  attachments = std::move(views);
  return std::move(*this);
}

auto FrameBuffer::Config::set_extent(VkExtent2D ext) && -> Config&& {
  extents = ext;
  return std::move(*this);
}

auto FrameBuffer::create(VkDevice device, RAW::Allocator allocator, VkRenderPass render_pass,
                         const std::vector<VkImageView>& attachments, VkExtent2D extents) -> Pointer {
  auto* frame_buffer = RAW::create_frame_buffer(device, allocator, render_pass, attachments, extents);

  return {
    frame_buffer,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto FrameBuffer::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.render_pass, config.attachments, config.extents);
}

}
