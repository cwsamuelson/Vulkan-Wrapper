#pragma once

namespace flp::Vulkan::Managed {

struct Surface {
  struct Deleter {
    using pointer = VkSurfaceKHR;

    VkInstance instance = nullptr;
    RAW::Allocator allocator = nullptr;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
  };

#ifdef _WIN32

  [[nodiscard]]
  static
  Pointer create(VkInstance instance, RAW::Allocator allocator, HWND hwnd, HINSTANCE hinstance);

#endif

  [[nodiscard]]
  static
  Pointer create(VkInstance instance, RAW::Allocator allocator, GLFWwindow* window);
};

}
