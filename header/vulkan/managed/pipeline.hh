#pragma once

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

namespace flp::Vulkan::Managed {

struct Pipeline {
  struct Deleter {
    using pointer = VkPipeline;

    VkDevice device;
    RAW::Allocator allocator;

    void operator()(pointer p);
  };

  using Pointer = std::unique_ptr<Deleter::pointer, Deleter>;

  struct Config {
    VkDevice device;
    RAW::Allocator allocator;
    std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
    VkPipelineVertexInputStateCreateInfo vertex_input_state;
    VkPipelineInputAssemblyStateCreateInfo input_assembly;
    VkPipelineViewportStateCreateInfo viewport_state;
    VkPipelineRasterizationStateCreateInfo rasterizer;
    VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineColorBlendStateCreateInfo color_blending;
    VkPipelineDynamicStateCreateInfo dynamic_state;
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;

    Config&& set_device(VkDevice dev) &&;
    Config&& set_allocator(RAW::Allocator alloc) &&;
    Config&& set_shader_stages(std::vector<VkPipelineShaderStageCreateInfo> stages) &&;
    Config&& set_vertex_input_state(VkPipelineVertexInputStateCreateInfo input_state) &&;
    Config&& set_input_assembly(VkPipelineInputAssemblyStateCreateInfo assembly) &&;
    Config&& set_viewport_state(VkPipelineViewportStateCreateInfo state) &&;
    Config&& set_rasterizer(VkPipelineRasterizationStateCreateInfo rast) &&;
    Config&& set_multisampling(VkPipelineMultisampleStateCreateInfo sampling) &&;
    Config&& set_color_blending(VkPipelineColorBlendStateCreateInfo blending) &&;
    Config&& set_dynamic_state(VkPipelineDynamicStateCreateInfo state) &&;
    Config&& set_pipeline_layout(VkPipelineLayout layout) &&;
    Config&& set_render_pass(VkRenderPass pass) &&;
  };

  [[nodiscard]]
  static
  Pointer create(
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

  [[nodiscard]]
  static
  Pointer create(const Config& config);
};

}
