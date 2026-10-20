from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake, CMakeDeps, CMakeToolchain


class LibConan(ConanFile):
    generators = 'CMakeToolchain'

    settings = "os", "compiler", "build_type", "arch"

    requires = [
        'boost/[^1.83.0]',
        'spdlog/[^1.12.0]',
        'magic_enum/[^0.9.5]',
        'glm/cci.20230113',
        'bitflags/[^1.5.0]',
        'vulkan-headers/[^1.3.268.0]',
        'neargye-semver/[^0.3.0]',
        #'vulkan-loader/1.3.268.0',# maybe better than system install?
        #'vulkan-validationlayers/1.3.239.0',
        # ('draco/1.5.6', 'override'),

        'error_support/[^1.2.1]',
    ]
    tool_requires = [
        'catch2/[^3.16.0]',
        'cmake/[^4]',
        'cmake_scripts/[^1.6.2]',
    ]
    default_options = {
        'boost/*:without_atomic': True,
        'boost/*:without_charconv': True,
        'boost/*:without_chrono': True,
        'boost/*:without_cobalt': True,
        'boost/*:without_container': True,
        'boost/*:without_context': True,
        'boost/*:without_contract': True,
        'boost/*:without_coroutine': True,
        'boost/*:without_date_time': True,
        'boost/*:without_fiber': True,
        'boost/*:without_filesystem': True,
        'boost/*:without_graph': True,
        'boost/*:without_graph_parallel': True,
        'boost/*:without_iostreams': True,
        'boost/*:without_json': True,
        'boost/*:without_locale': True,
        'boost/*:without_log': True,
        'boost/*:without_math': True,
        'boost/*:without_mpi': True,
        'boost/*:without_nowide': True,
        'boost/*:without_process': True,
        'boost/*:without_program_options': True,
        'boost/*:without_python': True,
        'boost/*:without_random': True,
        'boost/*:without_regex': True,
        'boost/*:without_serialization': True,
        'boost/*:without_stacktrace': True,
        'boost/*:without_system': True,
        'boost/*:without_test': True,
        'boost/*:without_thread': True,
        'boost/*:without_timer': True,
        'boost/*:without_type_erasure': True,
        'boost/*:without_url': True,
        'boost/*:without_wave': True,

        'boost/*:without_exception': False,

        'boost/*:bzip2': False,
        'boost/*:zlib': False,
        'boost/*:numa': False,
        'boost/*:lzma': False,
        'boost/*:zstd': False,

        'boost/*:pch': False,
        'boost/*:header_only': False,
        'boost/*:filesystem_use_std_fs': True,
        'boost/*:asio_no_deprecated': True,
        'boost/*:filesystem_no_deprecated': True,
    }

    def export_sources(self):
        git = Git(self)
        files = git.included_files()
        for file in files:
            copy(self, file, self.recipe_folder, self.export_sources_folder)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def generate(self):
        # tool_chain = CMakeToolchain(self)
        # tool_chain.generate()

        deps = CMakeDeps(self)
        deps.build_context_activated = ['cmake_scripts']
        deps.build_context_build_modules = ['cmake_scripts']
        deps.generate()

    def layout(self):
        cmake_layout(self)

    def package(self):
        cmake = CMake(self)
        cmake.install()
