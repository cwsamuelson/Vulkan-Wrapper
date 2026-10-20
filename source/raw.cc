#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include <windows.h>
#endif

#include "vulkan/raw.hh"

// #include <GLFW/glfw3.h>
// #include <assert_support/assert_support.hh>

#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <format>

namespace Vulkan::RAW {

DebugMessengerData::DebugMessengerData(
  VkDebugUtilsMessengerCreateInfoEXT* create_info,
  PFN_vkDebugUtilsMessengerCallbackEXT callback,
  void* data)
  : debug_messenger_create_info(create_info)
  , debug_callback(callback)
  , user_data(data)
{}

VkInstance create_instance(
  const std::string& app_name,
  const semver::version app_version,
  const std::vector<const char*>& extension_names,
  const std::vector<const char*>& validation_layer_names,
  Allocator pallocator,
  std::optional<DebugMessengerData> debug_messenger_data) {
  VkApplicationInfo application_info{};
  application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_info.pApplicationName = app_name.c_str();
  application_info.applicationVersion = VK_MAKE_VERSION(app_version.major, app_version.minor, app_version.patch);
  application_info.pEngineName = "No Engine";
  application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  application_info.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &application_info;
  create_info.enabledExtensionCount = extension_names.size();
  create_info.ppEnabledExtensionNames = extension_names.data();

  create_info.ppEnabledLayerNames = validation_layer_names.data();
  create_info.enabledLayerCount = validation_layer_names.size();

  if (debug_messenger_data) {
    auto* debug_create_info = debug_messenger_data->debug_messenger_create_info;
    auto debug_callback = debug_messenger_data->debug_callback;
    auto* user_data = debug_messenger_data->user_data;
    //debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    //debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    //debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info->pfnUserCallback = debug_callback;
    debug_create_info->pUserData = user_data;

    create_info.pNext = debug_create_info;
  }

  VkInstance instance{};

  //! @TODO add error info from vulkan runtime?
  VERIFY(vkCreateInstance(&create_info, pallocator, &instance) == VK_SUCCESS, "Couldn't create vulkan instance");

  return instance;
}

VkInstance create_instance(const std::string& app_name, semver::version app_version, const std::vector<const char*>& extension_names, const std::vector<const char*>& validation_layer_names, Allocator pallocator) {
  return create_instance(app_name, app_version, extension_names, validation_layer_names, pallocator, std::nullopt);
}

VkInstance create_instance(const std::string& app_name, semver::version app_version, const std::vector<const char*>& extension_names, const std::vector<const char*>& validation_layer_names) {
  return create_instance(app_name, app_version, extension_names, validation_layer_names, nullptr);
}

void destroy_instance(VkInstance instance, Allocator pallocator) {
  vkDestroyInstance(instance, pallocator);
}

VkDebugUtilsMessengerEXT create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, Allocator pallocator) {
  VkDebugUtilsMessengerEXT debug_messenger{};
  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
  VERIFY(func != nullptr, "Couldn't load vkCreateDebugUtilsMessengerEXT");

  auto result = func(instance, create_info, pallocator, &debug_messenger);
  VERIFY(result == VK_SUCCESS, "Failed to create the debug messenger");

  return debug_messenger;
}

VkDebugUtilsMessengerEXT create_debug_messenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info) {
  return create_debug_messenger(instance, create_info, nullptr);
}

void destroy_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, Allocator allocator) {
  auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
  VERIFY(func != nullptr, "Couldn't load vkDestroyDebugUtilsMessengerEXT");

  return func(instance, debug_messenger, allocator);
}

VkDevice create_device(VkPhysicalDevice physical_device, const VkDeviceCreateInfo* create_info, Allocator allocator) {
  VkDevice device;
  auto result = vkCreateDevice(physical_device, create_info, allocator, &device);
  VERIFY(result == VK_SUCCESS, "Failed to create logical device");

  return device;
}

void destroy_device(VkDevice device, Allocator allocator) {
  vkDestroyDevice(device, allocator);
}

#ifdef _WIN32

VkSurfaceKHR create_surface(VkInstance instance, Allocator allocator, HWND hwnd, HINSTANCE hinstance) {
  VkSurfaceKHR surface;

  VkWin32SurfaceCreateInfoKHR surface_create_info{};
  surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surface_create_info.hwnd = hwnd;
  surface_create_info.hinstance = hinstance;

  VERIFY(vkCreateWin32SurfaceKHR(instance, &surface_create_info, allocator, &surface) == VK_SUCCESS, "Couldn't create surface");

  return surface;
}

#endif

VkSurfaceKHR create_surface(VkInstance instance, GLFWwindow* window, Allocator allocator) {
  VkSurfaceKHR surface;
  VERIFY(glfwCreateWindowSurface(instance, window, allocator, &surface) == VK_SUCCESS, "Failed to create window surface");

  return surface;
}

void destroy_surface(VkInstance instance, Allocator allocator, VkSurfaceKHR surface) {
  vkDestroySurfaceKHR(instance, surface, allocator);
}

VkSwapchainKHR
create_swapchain(VkDevice device, VkSurfaceKHR surface, Allocator allocator, uint32_t image_count, VkFormat format,
                 VkColorSpaceKHR color_space, VkExtent2D extent, VkSurfaceTransformFlagBitsKHR pretransform,
                 VkPresentModeKHR present_mode, const std::vector<uint32_t>& queue_indices) {
  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

  create_info.surface = surface;
  create_info.minImageCount = image_count;
  create_info.imageFormat = format;
  create_info.imageColorSpace = color_space;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1;// unless stereo/VR
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  if (queue_indices.size() > 1) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = queue_indices.size();
    create_info.pQueueFamilyIndices = queue_indices.data();
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;
    create_info.pQueueFamilyIndices = nullptr;
  }

  create_info.preTransform = pretransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;

  VkSwapchainKHR swapchain;
  VERIFY(vkCreateSwapchainKHR(device, &create_info, allocator, &swapchain) == VK_SUCCESS, "Failed to create swapchain");
  return swapchain;
}

void destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain, Allocator allocator) {
  vkDestroySwapchainKHR(device, swapchain, allocator);
}

VkImageView create_image_view(VkDevice device, RAW::Allocator allocator, VkImage image, VkFormat format) {
  VkImageViewCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  create_info.image = image;
  create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  create_info.format = format;
  create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  create_info.subresourceRange.baseMipLevel = 0;
  create_info.subresourceRange.levelCount = 1;
  create_info.subresourceRange.baseArrayLayer = 0;
  create_info.subresourceRange.layerCount = 1;// usable for VR/stereographic

  VkImageView image_view;

  VERIFY(vkCreateImageView(device, &create_info, allocator, &image_view) == VK_SUCCESS, "Failed to create image views");

  return image_view;
}

void destroy_image_view(VkDevice device, VkImageView image_view, RAW::Allocator allocator) {
  vkDestroyImageView(device, image_view, allocator);
}

VkShaderModule create_shader_module(VkDevice device, RAW::Allocator allocator, const std::vector<char>& code) {
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode =  reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shader_module;
  VERIFY(vkCreateShaderModule(device, &create_info, allocator, &shader_module) == VK_SUCCESS, "");

  return shader_module;
}

void destroy_shader_module(VkDevice device, RAW::Allocator allocator, VkShaderModule shader_module) {
  vkDestroyShaderModule(device, shader_module, allocator);
}

VkPipelineLayout create_pipeline_layout(VkDevice device, RAW::Allocator allocator) {
  VkPipelineLayoutCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.setLayoutCount = 0;
  create_info.pSetLayouts = nullptr;
  create_info.pushConstantRangeCount = 0;
  create_info.pPushConstantRanges = nullptr;

  VkPipelineLayout pipeline_layout;
  VERIFY(vkCreatePipelineLayout(device, &create_info, allocator, &pipeline_layout) == VK_SUCCESS, "");

  return pipeline_layout;
}

void destroy_pipeline_layout(VkDevice device, RAW::Allocator allocator, VkPipelineLayout pipeline_layout) {
  vkDestroyPipelineLayout(device, pipeline_layout, allocator);
}

VkRenderPass create_render_pass(VkDevice device,
                                RAW::Allocator allocator,
                                const std::vector<VkAttachmentDescription>& attachments,
                                const std::vector<VkSubpassDescription>& sub_passes,
                                const std::vector<VkSubpassDependency>& dependencies) {
  VkRenderPassCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  create_info.attachmentCount = attachments.size();
  create_info.pAttachments = attachments.data();
  create_info.subpassCount = sub_passes.size();
  create_info.pSubpasses = sub_passes.data();
  create_info.dependencyCount = dependencies.size();
  create_info.pDependencies = dependencies.data();

  VkRenderPass render_pass;
  VERIFY(vkCreateRenderPass(device, &create_info, allocator, &render_pass) == VK_SUCCESS, "");

  return render_pass;
}

void destroy_render_pass(VkDevice device, RAW::Allocator allocator, VkRenderPass render_pass) {
  vkDestroyRenderPass(device, render_pass, allocator);
}

VkPipeline create_pipeline(VkDevice device,
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
                           VkRenderPass render_pass) {
  VkGraphicsPipelineCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  create_info.stageCount = shader_stages.size();
  create_info.pStages = shader_stages.data();

  create_info.pVertexInputState = &vertex_input_state;
  create_info.pInputAssemblyState = &input_assembly;
  create_info.pViewportState = &viewport_state;
  create_info.pRasterizationState = &rasterizer;
  create_info.pMultisampleState = &multisampling;
  create_info.pDepthStencilState = nullptr;
  create_info.pColorBlendState = &color_blending;
  create_info.pDynamicState = &dynamic_state;

  create_info.layout = pipeline_layout;
  create_info.renderPass = render_pass;
  create_info.subpass = 0;

  create_info.basePipelineHandle = VK_NULL_HANDLE;
  create_info.basePipelineIndex = -1;

  VkPipeline pipeline;
  VERIFY(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &create_info, allocator, &pipeline) == VK_SUCCESS, "");

  return pipeline;
}

void destroy_pipeline(VkDevice device, RAW::Allocator allocator, VkPipeline pipeline) {
  vkDestroyPipeline(device, pipeline, allocator);
}

VkFramebuffer create_frame_buffer(VkDevice device,
                                  RAW::Allocator allocator,
                                  VkRenderPass render_pass,
                                  const std::vector<VkImageView>& attachments,
                                  VkExtent2D extents) {
  VkFramebufferCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  create_info.renderPass = render_pass;
  create_info.attachmentCount = attachments.size();
  create_info.pAttachments = attachments.data();
  create_info.width = extents.width;
  create_info.height = extents.height;
  create_info.layers = 1;

  VkFramebuffer frame_buffer;
  VERIFY(vkCreateFramebuffer(device, &create_info, allocator, &frame_buffer) == VK_SUCCESS, "");

  return frame_buffer;
}

void destroy_frame_buffer(VkDevice device, RAW::Allocator allocator, VkFramebuffer frame_buffer) {
  vkDestroyFramebuffer(device, frame_buffer, allocator);
}

VkCommandPool create_command_pool(VkDevice device, RAW::Allocator allocator, uint32_t queue_family_index) {
  VkCommandPoolCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  create_info.queueFamilyIndex = queue_family_index;

  VkCommandPool command_pool;
  VERIFY(vkCreateCommandPool(device, &create_info, allocator, &command_pool) == VK_SUCCESS, "");

  return command_pool;
}

void destroy_command_pool(VkDevice device, RAW::Allocator allocator, VkCommandPool command_pool) {
  vkDestroyCommandPool(device, command_pool, allocator);
}

VkSemaphore create_semaphore(VkDevice device, RAW::Allocator allocator) {
  VkSemaphoreCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkSemaphore semaphore;
  VERIFY(vkCreateSemaphore(device, &create_info, allocator, &semaphore) == VK_SUCCESS, "");

  return semaphore;
}

void destroy_semaphore(VkDevice device, RAW::Allocator allocator, VkSemaphore semaphore) {
  vkDestroySemaphore(device, semaphore, allocator);
}

VkFence create_fence(VkDevice device, RAW::Allocator allocator, VkFlags flags) {
  VkFenceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  create_info.flags = flags;

  VkFence fence;
  VERIFY(vkCreateFence(device, &create_info, allocator, &fence) == VK_SUCCESS, "");

  return fence;
}

void destroy_fence(VkDevice device, RAW::Allocator allocator, VkFence fence) {
  vkDestroyFence(device, fence, allocator);
}

}
