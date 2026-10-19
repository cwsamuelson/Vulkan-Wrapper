#include "vulkan/managed/instance.hh"

namespace flp::Vulkan::Managed {

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

}
