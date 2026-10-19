from conans import ConanFile, tools, CMake


class LibConan(ConanFile):
    generators = "cmake_find_package"

    requires = [
        "boost/1.83.0",
        "spdlog/1.12.0",
        "magic_enum/0.9.5",
        "glm/cci.20230113",
        "bitflags/1.5.0",
        "vulkan-headers/1.3.268.0",
        "neargye-semver/0.3.0",
        #"vulkan-loader/1.3.268.0",# maybe better than system install?
        #"vulkan-validationlayers/1.3.239.0",
        # ("draco/1.5.6", "override"),
    ]
    tool_requires = [
        "catch2/[^3.16.0]",
    ]
    default_options = {
        "boost:bzip2": False,
        "boost:zlib": False,
        "boost:without_test": True,
        "boost:without_iostreams": True,
        "boost:without_fiber": True,
        "boost:without_nowide": True,
        "boost:without_wave": True,
        "boost:without_log": True,
        "boost:without_python": True,
        "boost:filesystem_use_std_fs": True,
    }

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
