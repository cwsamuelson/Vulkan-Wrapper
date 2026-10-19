#pragma once

namespace flp::Vulkan::Managed {

struct ImageView {
  struct Deleter {
    using pointer = VkImageView;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    VkImage image;
    VkFormat format;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_image(VkImage img) &&;
    Config&& set_format(VkFormat fmt) &&;
  };

  [[nodiscard]]
  static
  Pointer create(VkDevice device, RAW::Allocator allocator, VkImage image, VkFormat format);

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
