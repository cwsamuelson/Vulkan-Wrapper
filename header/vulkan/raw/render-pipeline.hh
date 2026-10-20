#pragma once

#include <vulkan/vulkan_core.h>

#include <vector>

namespace flp::Vulkan::RAW {

VkPipeline create_pipeline(
  VkDevice device,
  RAW::Allocator allocator,
  const std::vector<VkPipelineShaderStageCreateInfo>& shader_stages,
  VkPipelineVertexInputStateCreateInfo vertex_input_state,
  VkPipelineInputAssemblyStateCreateInfo input_assembly,
  VkPipelineViewportStateCreateInfo viewport_state,
  VkPipelineRasterizationStateCreateInfo rasterizer,
  VkPipelineMultisampleStateCreateInfo multisampling,
  VkPipelineColorBlendStateCreateInfo color_blending,
  VkPipelineDynamicStateCreateInfo dynamic_state,
  VkPipelineLayout pipeline_layout,
  VkRenderPass render_pass);

void destroy_pipeline(VkDevice device, RAW::Allocator allocator, VkPipeline pipeline);

}
