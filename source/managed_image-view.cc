#include "vulkan/managed/image-view.hh"

namespace flp::Vulkan::Managed {

void ImageView::Deleter::operator()(pointer p) {
  RAW::destroy_image_view(device, p, allocator);
}

auto ImageView::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto ImageView::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto ImageView::Config::set_image(VkImage img) && -> Config&& {
  image = img;
  return std::move(*this);
}

auto ImageView::Config::set_format(VkFormat fmt) && -> Config&& {
  format = fmt;
  return std::move(*this);
}

auto ImageView::create(VkDevice device, RAW::Allocator allocator, VkImage image, VkFormat format) -> Pointer {
  auto* image_view = RAW::create_image_view(device, allocator, image, format);

  return {image_view,
          {
            .device = device,
            .allocator = allocator,
          }
  };
}

auto ImageView::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.image, config.format);
}

}
