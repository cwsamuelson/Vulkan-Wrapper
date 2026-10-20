#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace flp::Vulkan::RAW {

VkShaderModule create_shader_module(VkDevice device, RAW::Allocator allocator, const std::vector<char>& code);

void destroy_shader_module(VkDevice device, RAW::Allocator allocator, VkShaderModule shader_module);

}
