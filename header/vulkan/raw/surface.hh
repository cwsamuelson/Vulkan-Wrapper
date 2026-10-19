#pragma once

namespace flp::Vulkan::raw {

#ifdef _WIN32

VkSurfaceKHR create_surface(VkInstance instance, Allocator allocator, HWND hwnd, HINSTANCE hinstance);

#endif

// glfw specific? :(
VkSurfaceKHR create_surface(VkInstance instance, GLFWwindow* window, Allocator allocator);

void destroy_surface(VkInstance instance, Allocator allocator, VkSurfaceKHR surface);

}
