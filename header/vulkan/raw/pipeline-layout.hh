#pragma once

namespace flp::Vulkan::raw {

VkPipelineLayout create_pipeline_layout(VkDevice device, RAW::Allocator allocator);

void destroy_pipeline_layout(VkDevice device, RAW::Allocator allocator, VkPipelineLayout pipeline_layout);

}
