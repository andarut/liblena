from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

class LibLenaRecipe(ConanFile):
	name = "liblena"
	version = "0.1"
	settings = "os", "compiler", "build_type", "arch"

	options = {"shared": [True, False], "fPIC": [True, False]}
	default_options = {"shared": False, "fPIC": True}

	def build_requirements(self):
		self.tool_requires("cmake/3.23.5")
		self.test_requires("gtest/1.13.0")

	def generate(self):
		tc = CMakeToolchain(self, generator="Ninja")
		if self.settings.build_type == "Debug":
			# TODO: ASAN_OPTIONS="detect_leaks=1"
			tc.variables["CMAKE_C_FLAGS"] = "-fsanitize=address -fno-omit-frame-pointer"
			tc.variables["CMAKE_CXX_FLAGS"] = "-fsanitize=address -fno-omit-frame-pointer"
		tc.generate()
		deps = CMakeDeps(self)
		deps.generate()

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def layout(self):
		cmake_layout(self)