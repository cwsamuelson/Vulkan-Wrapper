#pragma once

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#include <windows.h>
#endif
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <semver.hpp>
#include <vulkan/vulkan_core.h>

#include "vulkan/raw/allocator.hh"
#include "vulkan/raw/command-pool.hh"
#include "vulkan/raw/debug-messenger.hh"
#include "vulkan/raw/device.hh"
#include "vulkan/raw/fence.hh"
#include "vulkan/raw/frame-buffer.hh"
#include "vulkan/raw/image-view.hh"
#include "vulkan/raw/instance.hh"
#include "vulkan/raw/pipeline-layout.hh"
#include "vulkan/raw/render-pass.hh"
#include "vulkan/raw/render-pipeline.hh"
#include "vulkan/raw/semaphore.hh"
#include "vulkan/raw/shader-module.hh"
#include "vulkan/raw/surface.hh"
#include "vulkan/raw/swap-chain.hh"

#include <string>
#include <vector>

namespace Vulkan::RAW {

}
