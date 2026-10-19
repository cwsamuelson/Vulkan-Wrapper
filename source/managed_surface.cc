#include "vulkan/managed/surface.hh"

namespace flp::Vulkan::Managed {

void Surface::Deleter::operator()(Vulkan::Surface::Deleter::pointer p) {
  RAW::destroy_surface(instance, allocator, p);
}

#ifdef _WIN32

auto Surface::create(VkInstance instance, RAW::Allocator allocator, HWND hwnd, HINSTANCE hinstance) -> Pointer {
  auto surface = RAW::create_surface(instance, allocator, hwnd, hinstance);

  return {surface,
          {
            .instance = instance,
            .allocator = allocator,
          }};
}

#endif

auto Surface::create(VkInstance instance, RAW::Allocator allocator, GLFWwindow* window) -> Pointer {
  auto surface = RAW::create_surface(instance, window, allocator);

  return {surface,
          {
            .instance = instance,
            .allocator = allocator,
          }};
}

}
