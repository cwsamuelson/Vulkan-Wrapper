#include "vulkan/managed.hh"
#include "vulkan/raw.hh"

#include <vulkan/vulkan_core.h>
#include <semver.hpp>

#include <algorithm>
#include <iterator>
#include <string_view>

namespace Vulkan {

void Instance::Deleter::operator()(Vulkan::Instance::Deleter::pointer p) {
  RAW::destroy_instance(p, allocator);
}

auto Instance::Config::set_app_name(std::string_view name)&& -> Config&& {
  app_name = name;
  return std::move(*this);
}

auto Instance::Config::set_version(semver::version version)&& -> Config&& {
  app_version = version;
  return std::move(*this);
}

auto Instance::Config::set_extension_names(std::vector<const char*> names) && -> Config&& {
  extension_names = std::move(names);
  return std::move(*this);
}

auto Instance::Config::set_validation_layer_names(std::vector<const char*> names) && -> Config&& {
  validation_layer_names = std::move(names);
  return std::move(*this);
}

auto Instance::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto Instance::Config::set_debug_messenger(std::optional<RAW::DebugMessengerData> messenger) && -> Config&& {
  debug_messenger = messenger;
  return std::move(*this);
}

auto Instance::create(const Vulkan::Instance::Config &config) -> Pointer {
  return create(config.app_name, config.app_version, config.extension_names, config.validation_layer_names, config.allocator, config.debug_messenger);
}

auto Instance::create(const std::string &app_name, semver::version app_version,
                         const std::vector<const char*> &extension_names,
                         const std::vector<const char*> &validation_layer_names, RAW::Allocator allocator,
                         std::optional<RAW::DebugMessengerData> debug_messenger) -> Pointer {
  auto instance = RAW::create_instance(app_name, app_version, extension_names, validation_layer_names, allocator, debug_messenger);
  return {instance, {
    .allocator = allocator,
  }};
}

void DebugMessenger::Deleter::operator()(Vulkan::DebugMessenger::Deleter::pointer p) {
  RAW::destroy_debug_messenger(instance, p, allocator);
}

auto DebugMessenger::Config::set_create_info(const VkDebugUtilsMessengerCreateInfoEXT* info) && -> Config&& {
  create_info = info;
  return std::move(*this);
}

auto DebugMessenger::Config::set_allocator(RAW::Allocator alloc) && -> Config&&{
  allocator = alloc;
  return std::move(*this);
}

auto DebugMessenger::create(const Vulkan::DebugMessenger::Config& config) -> Pointer {
  return create(config.instance, config.create_info, config.allocator);
}

auto DebugMessenger::create(const Instance::Pointer& instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info,
                               RAW::Allocator allocator) -> Pointer {
  auto* debug_messenger = RAW::create_debug_messenger(instance.get(), create_info, allocator);
  return {debug_messenger, {
      .instance = instance.get(),
      .allocator = allocator,
  }};
}

void LogicalDevice::Deleter::operator()(Vulkan::LogicalDevice::Deleter::pointer p) {
  RAW::destroy_device(p, allocator);
}

auto LogicalDevice::Config::set_allocator(RAW::Allocator alloc)&& -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto LogicalDevice::Config::set_device(VkPhysicalDevice device) && -> Config&& {
  physical_device = device;
  return std::move(*this);
}

auto LogicalDevice::Config::set_family(std::vector<uint32_t> families) && -> Config&& {
  family_indices = families;
  return std::move(*this);
}

auto LogicalDevice::Config::set_extensions(std::vector<const char*> exts) && -> Config&& {
  extensions = exts;
  return std::move(*this);
}

auto LogicalDevice::create(const Vulkan::LogicalDevice::Config& config) -> Pointer {
  return create(config.physical_device, config.allocator, config.family_indices, config.extensions);
}

auto LogicalDevice::create(VkPhysicalDevice physical_device,
                           RAW::Allocator allocator,
                           const std::vector<uint32_t>& family_indices,
                           const std::vector<const char*>& extensions) -> Pointer {
  const float priority = 1.0f;

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

  std::ranges::transform(family_indices, std::back_inserter(queue_create_infos), [&priority](const uint32_t index) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &priority;

    return queue_create_info;
  });

  VkPhysicalDeviceFeatures features{};

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.queueCreateInfoCount = queue_create_infos.size();
  device_create_info.pEnabledFeatures = &features;
  device_create_info.ppEnabledExtensionNames = extensions.data();
  device_create_info.enabledExtensionCount = extensions.size();

  return create(physical_device, &device_create_info, allocator);
}

auto LogicalDevice::create(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* const device_create_info,
                              RAW::Allocator allocator) -> Pointer {
  auto* device = RAW::create_device(physical_device, device_create_info, allocator);

  return {device, {
      .allocator = allocator
  }};
}

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

void Swapchain::Deleter::operator()(Vulkan::Swapchain::Deleter::pointer p) {
  RAW::destroy_swapchain(device, p, allocator);
}

auto Swapchain::Config::set_device(VkDevice dev)&& -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Swapchain::Config::set_surface(VkSurfaceKHR srfc) && -> Config&& {
  surface = srfc;
  return std::move(*this);
}

auto Swapchain::Config::set_allocator(RAW::Allocator alc) && -> Config&& {
  allocator = alc;
  return std::move(*this);
}

auto Swapchain::Config::set_image_count(uint32_t count) && -> Config&& {
  image_count = count;
  return std::move(*this);
}

auto Swapchain::Config::set_format(VkFormat fmt) && -> Config&& {
  format = fmt;
  return std::move(*this);
}

auto Swapchain::Config::set_color_space(VkColorSpaceKHR space) && -> Config&& {
  color_space = space;
  return std::move(*this);
}

auto Swapchain::Config::set_extent(VkExtent2D ext) && -> Config&& {
  extent = ext;
  return std::move(*this);
}

auto Swapchain::Config::set_pretransform(VkSurfaceTransformFlagBitsKHR transform) && -> Config&& {
  pretransform = transform;
  return std::move(*this);
}

auto Swapchain::Config::set_present_mode(VkPresentModeKHR mode) && -> Config&& {
  present_mode = mode;
  return std::move(*this);
}

auto Swapchain::Config::set_queue_indices(std::vector<uint32_t> indices) && -> Config&& {
  queue_indices = std::move(indices);
  return std::move(*this);
}

auto Swapchain::create(VkDevice device, VkSurfaceKHR surface, RAW::Allocator allocator, uint32_t image_count,
                       VkFormat format, VkColorSpaceKHR color_space, VkExtent2D extent,
                       VkSurfaceTransformFlagBitsKHR pretransform, VkPresentModeKHR present_mode,
                       const std::vector<uint32_t>& queue_indices) -> Pointer {
  auto* swapchain = RAW::create_swapchain(device, surface, allocator, image_count, format, color_space, extent,
                                         pretransform, present_mode, queue_indices);

  return {swapchain,
          {
            .device = device,
            .allocator = allocator,
          }};
}

auto Swapchain::create(const Vulkan::Swapchain::Config& config) -> Pointer {
  return create(config.device, config.surface, config.allocator, config.image_count, config.format,
                config.color_space, config.extent, config.pretransform, config.present_mode, config.queue_indices);
}

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

void ShaderModule::Deleter::operator()(Vulkan::ShaderModule::Deleter::pointer p) {
  RAW::destroy_shader_module(device, allocator, p);
}

auto ShaderModule::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto ShaderModule::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto ShaderModule::Config::set_shader_code(std::vector<char> code) && -> Config&& {
  shader_code = std::move(code);
  return std::move(*this);
}

auto ShaderModule::create(VkDevice device, RAW::Allocator allocator, const std::vector<char>& shader_code) -> Pointer {
  auto* shader_module = RAW::create_shader_module(device, allocator, shader_code);

  return {
    shader_module,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto ShaderModule::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.shader_code);
}

void PipelineLayout::Deleter::operator()(pointer p) {
  RAW::destroy_pipeline_layout(device, allocator, p);
}

auto PipelineLayout::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto PipelineLayout::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto PipelineLayout::create(VkDevice device, RAW::Allocator allocator) -> Pointer {
  auto* layout = RAW::create_pipeline_layout(device, allocator);

  return {
    layout,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto PipelineLayout::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator);
}

void RenderPass::Deleter::operator()(pointer p) {
  RAW::destroy_render_pass(device, allocator, p);
}

auto RenderPass::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto RenderPass::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto RenderPass::Config::set_attachments(std::vector<VkAttachmentDescription> attach) && -> Config&& {
  attachments = std::move(attach);
  return std::move(*this);
}

auto RenderPass::Config::set_sub_passes(std::vector<VkSubpassDescription> passes) && -> Config&& {
  sub_passes = std::move(passes);
  return std::move(*this);
}

auto RenderPass::Config::set_dependencies(std::vector<VkSubpassDependency> deps) && -> Config&& {
  dependencies = std::move(deps);
  return std::move(*this);
}

auto RenderPass::create(VkDevice device,
                        RAW::Allocator allocator,
                        const std::vector<VkAttachmentDescription>& attachments,
                        const std::vector<VkSubpassDescription>& sub_passes,
                        const std::vector<VkSubpassDependency>& dependencies) -> Pointer {
  auto* render_pass = RAW::create_render_pass(device, allocator, attachments, sub_passes, dependencies);

  return {
    render_pass,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto RenderPass::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.attachments, config.sub_passes, config.dependencies);
}

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

auto Pipeline::create(VkDevice device, RAW::Allocator allocator,
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

void FrameBuffer::Deleter::operator()(pointer p) {
  RAW::destroy_frame_buffer(device, allocator, p);
}

auto FrameBuffer::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto FrameBuffer::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto FrameBuffer::Config::set_render_pass(VkRenderPass pass) && -> Config&& {
  render_pass = pass;
  return std::move(*this);
}

auto FrameBuffer::Config::set_attachments(std::vector<VkImageView> views) && -> Config&& {
  attachments = std::move(views);
  return std::move(*this);
}

auto FrameBuffer::Config::set_extent(VkExtent2D ext) && -> Config&& {
  extents = ext;
  return std::move(*this);
}

auto FrameBuffer::create(VkDevice device, RAW::Allocator allocator, VkRenderPass render_pass,
                         const std::vector<VkImageView>& attachments, VkExtent2D extents) -> Pointer {
  auto* frame_buffer = RAW::create_frame_buffer(device, allocator, render_pass, attachments, extents);

  return {
    frame_buffer,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto FrameBuffer::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.render_pass, config.attachments, config.extents);
}

void CommandPool::Deleter::operator()(pointer p) {
  RAW::destroy_command_pool(device, allocator, p);
}

auto CommandPool::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto CommandPool::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto CommandPool::Config::set_queue_family_index(const uint32_t index) && -> Config&& {
  queue_family_index = index;
  return std::move(*this);
}

auto CommandPool::create(VkDevice device, RAW::Allocator allocator, const uint32_t queue_family_index) -> Pointer {
  auto* command_pool = RAW::create_command_pool(device, allocator, queue_family_index);

  return {
    command_pool,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto CommandPool::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.queue_family_index);
}

void Semaphore::Deleter::operator()(pointer p) {
  RAW::destroy_semaphore(device, allocator, p);
}

auto Semaphore::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Semaphore::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto Semaphore::create(VkDevice device, RAW::Allocator allocator) -> Pointer {
  auto* semaphore = RAW::create_semaphore(device, allocator);

  return {
    semaphore,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto Semaphore::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator);
}

void Fence::Deleter::operator()(pointer p) {
  RAW::destroy_fence(device, allocator, p);
}

auto Fence::Config::set_device(VkDevice dev) && -> Config&& {
  device = dev;
  return std::move(*this);
}

auto Fence::Config::set_allocator(RAW::Allocator alloc) && -> Config&& {
  allocator = alloc;
  return std::move(*this);
}

auto Fence::Config::set_flags(VkFlags flgs) && -> Config&& {
  flags = flgs;
  return std::move(*this);
}

auto Fence::create(VkDevice device, RAW::Allocator allocator, VkFlags flags) -> Pointer {
  auto* fence = RAW::create_fence(device, allocator, flags);

  return {
    fence,
    {
      .device = device,
      .allocator = allocator,
    }
  };
}

auto Fence::create(const Config& config) -> Pointer {
  return create(config.device, config.allocator, config.flags);
}

}
