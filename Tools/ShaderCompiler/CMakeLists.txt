set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(shaderc REQUIRED)

add_executable(ShaderCompiler
	"src/compiler_main.cpp"
)

target_link_libraries(ShaderCompiler
	PRIVATE
		shaderc::shaderc
)