#include "vulkan/managed/pipeline.hh"

namespace flp::Vulkan::Managed {

void Pipeline::Deleter::operator()(pointer p) {
  RAW::destroy_pipeline(device, allocator, p);
}

auto Pipeline::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Pipeline::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto Pipeline::Config::set_shader_stages(std::vector<VkPipelineShaderStageCreateInfo> stages) && -> Pipeline::Config&& {
  shader_stages = std::move(stages);
  return std::move(*this);
}

auto Pipeline::Config::set_vertex_input_state(VkPipelineVertexInputStateCreateInfo input_state) && -> Pipeline::Config&& {
  vertex_input_state = input_state;
  return std::move(*this);
}

auto Pipeline::Config::set_input_assembly(VkPipelineInputAssemblyStateCreateInfo assembly) && -> Pipeline::Config&& {
  input_assembly = assembly;
  return std::move(*this);
}

auto Pipeline::Config::set_viewport_state(VkPipelineViewportStateCreateInfo state) && -> Pipeline::Config&& {
  viewport_state = state;
  return std::move(*this);
}

auto Pipeline::Config::set_rasterizer(VkPipelineRasterizationStateCreateInfo rast) && -> Pipeline::Config&& {
  rasterizer = rast;
  return std::move(*this);
}

auto Pipeline::Config::set_multisampling(VkPipelineMultisampleStateCreateInfo sampling) && -> Pipeline::Config&& {
  multisampling = sampling;
  return std::move(*this);
}

auto Pipeline::Config::set_color_blending(VkPipelineColorBlendStateCreateInfo blending) && -> Pipeline::Config&& {
  color_blending = blending;
  return std::move(*this);
}

auto Pipeline::Config::set_dynamic_state(VkPipelineDynamicStateCreateInfo state) && -> Pipeline::Config&& {
  dynamic_state = state;
  return std::move(*this);
}

auto Pipeline::Config::set_pipeline_layout(VkPipelineLayout layout) && -> Pipeline::Config&& {
  pipeline_layout = layout;
  return std::move(*this);
}

auto Pipeline::Config::set_render_pass(VkRenderPass pass) && -> Pipeline::Config&& {
  render_pass = pass;
  return std::move(*this);
}

auto Pipeline::create(
  VkDevice device, RAW::Allocator allocator,
  const std::vector<VkPipelineShaderStageCreateInfo>& shader_stages,
  VkPipelineVertexInputStateCreateInfo vertex_input_state,
  VkPipelineInputAssemblyStateCreateInfo input_assembly,
  VkPipelineViewportStateCreateInfo viewport_state,
  VkPipelineRasterizationStateCreateInfo rasterizer,
  VkPipelineMultisampleStateCreateInfo multisampling,
  VkPipelineColorBlendStateCreateInfo color_blending,
  VkPipelineDynamicStateCreateInfo dynamic_state,
  VkPipelineLayout pipeline_layout,
  VkRenderPass render_pass) -> Pointer {
  auto* pipeline = RAW::create_pipeline(device, allocator, shader_stages, vertex_input_state, input_assembly,
                                                     viewport_state, rasterizer, multisampling, color_blending,
                                                     dynamic_state, pipeline_layout, render_pass);

  return {
    pipeline,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto Pipeline::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.shader_stages, config.vertex_input_state, config.input_assembly,
                config.viewport_state, config.rasterizer, config.multisampling, config.color_blending, config.dynamic_state,
                config.pipeline_layout, config.render_pass);
}

}
