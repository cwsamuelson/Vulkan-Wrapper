cmake_minimum_required(VERSION 4.0)
project(${DIR_NAME})

find_package(VulkanHeaders REQUIRED)
find_package(semver REQUIRED)
find_package(glfw3 REQUIRED)

set(HEADER_FILES
  header/vulkan/managed.hh
  header/vulkan/managed/command-pool.hh
  header/vulkan/managed/debug-messenger.hh
  header/vulkan/managed/fence.hh
  header/vulkan/managed/frame-buffer.hh
  header/vulkan/managed/image-view.hh
  header/vulkan/managed/instance.hh
  header/vulkan/managed/logical-device.hh
  header/vulkan/managed/pipeline-layout.hh
  header/vulkan/managed/pipeline.hh
  header/vulkan/managed/render-pass.hh
  header/vulkan/managed/semaphore.hh
  header/vulkan/managed/shader-module.hh
  header/vulkan/managed/surface.hh
  header/vulkan/managed/swap-chain.hh
  header/vulkan/raw.hh
  header/vulkan/raw/allocator.hh
  header/vulkan/raw/command-pool.hh
  header/vulkan/raw/debug-messenger.hh
  header/vulkan/raw/device.hh
  header/vulkan/raw/fence.hh
  header/vulkan/raw/frame-buffer.hh
  header/vulkan/raw/image-view.hh
  header/vulkan/raw/instance.hh
  header/vulkan/raw/pipeline-layout.hh
  header/vulkan/raw/render-pass.hh
  header/vulkan/raw/render-pipeline.hh
  header/vulkan/raw/semaphore.hh
  header/vulkan/raw/shader-module.hh
  header/vulkan/raw/surface.hh
  header/vulkan/raw/swap-chain.hh
  header/vulkan/structured.hh
)
set(SOURCE_FILES
  source/raw.cc
  source/managed.cc
  source/structured.cc
)

common_library(${PROJECT_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME}
PUBLIC
  header
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
  Vulkan::Headers
  semver::semver
  glfw::glfw

  assert_support
)
