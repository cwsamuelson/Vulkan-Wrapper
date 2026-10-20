#pragma once

#include <vulkan/vulkan_core.h>

namespace flp::Vulkan::RAW {

VkPipelineLayout create_pipeline_layout(VkDevice device, RAW::Allocator allocator);

void destroy_pipeline_layout(VkDevice device, RAW::Allocator allocator, VkPipelineLayout pipeline_layout);

}
