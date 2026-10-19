#pragma once

#include "vulkan/managed/command-pool.hh"
#include "vulkan/managed/debug-messenger.hh"
#include "vulkan/managed/fence.hh"
#include "vulkan/managed/frame-buffer.hh"
#include "vulkan/managed/image-view.hh"
#include "vulkan/managed/instance.hh"
#include "vulkan/managed/logical-device.hh"
#include "vulkan/managed/pipeline-layout.hh"
#include "vulkan/managed/pipeline.hh"
#include "vulkan/managed/render-pass.hh"
#include "vulkan/managed/semaphore.hh"
#include "vulkan/managed/shader-module.hh"
#include "vulkan/managed/surface.hh"
#include "vulkan/managed/swap-chain.hh"

#include <vulkan/vulkan_core.h>
#include <semver.hpp>

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

/*struct DataWrapper {
  std::optional<Data> data;
  struct Deleter {
    using pointer = DataWrapper;
    void operator()(pointer){}
  };

  DataWrapper() = default;
  DataWrapper(std::optional<Data> d)
    : data(std::move(d))
  {}
  DataWrapper(Data d)
    : data(std::move(d))
  {}

  friend bool operator==(const DataWrapper& wrapper, std::nullptr_t) {
    return wrapper.data.has_value();   
  }

  Data& operator*() {
    return *data;
  }
  Data* operator->() {
    return &(*data);
  }
};*/
//using PointerS = std::unique_ptr<Data, DataWrapper::Deleter>;

