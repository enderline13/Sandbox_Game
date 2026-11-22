#include <shaderc/shaderc.hpp>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <print>

namespace fs = std::filesystem;

static shaderc_shader_kind get_shader_kind(const fs::path& ext) {
    if (ext == ".vert") return shaderc_vertex_shader;
    if (ext == ".frag") return shaderc_fragment_shader;
    if (ext == ".comp") return shaderc_compute_shader;
    if (ext == ".geom") return shaderc_geometry_shader;
    return shaderc_glsl_infer_from_source;
}

static bool compile_file(
    shaderc::Compiler& compiler,
    const shaderc::CompileOptions& options,
    const fs::path& input_path,
    const fs::path& output_path
) {
    std::ifstream in_file(input_path, std::ios::ate | std::ios::binary);
    if (!in_file.is_open()) {
        std::print("[File Error] Cannot open: <{}>\n", input_path.string());
        return false;
    }

    const size_t size = static_cast<size_t>(in_file.tellg());
    std::string source(size, '\0');
    in_file.seekg(0);
    in_file.read(source.data(), size);

    const shaderc_shader_kind kind = get_shader_kind(input_path.extension());
    const std::string filename = input_path.filename().string();

    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, kind, filename.c_str(), options);
    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        std::print("[Shader Error] <{}>: {}\n", filename, result.GetErrorMessage());
        return false;
    }

    if (result.GetNumWarnings() > 0) {
        std::print("[Shader Warning!] <{}>: {}\n", filename, result.GetErrorMessage());
    }

    fs::create_directories(output_path.parent_path());
    std::ofstream out_file(output_path, std::ios::binary);
    if (!out_file.is_open()) {
        std::print("[File Error] Cannot write: <{}>\n", output_path.string());
        return false;
    }

    const std::vector<uint32_t> spirv(result.cbegin(), result.cend());
    out_file.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
    return true;
}

int main(int argc, char** argv) {
	if (argc < 3) {
		std::print("Correct usage: ShaderCompiler <input_dir> <output_dir> <debug/release>");
		return 1;
	}

	const fs::path input_dir = argv[1];
	const fs::path output_dir = argv[2];
	const bool debug_mode = (argc > 3 && std::string(argv[3]) == "debug");

	shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

    if (debug_mode) {
        options.SetGenerateDebugInfo();
        options.SetOptimizationLevel(shaderc_optimization_level_zero);
    } else {
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    int compiled = 0;
    int failed = 0;

    for (const auto& entry : fs::recursive_directory_iterator(input_dir)) {
        if (!entry.is_regular_file()) continue;

        const fs::path ext = entry.path().extension();
        if (ext != ".vert" && ext != ".frag" && ext != ".comp" && ext != ".geom") continue;

        fs::path rel_path = fs::relative(entry.path(), input_dir); // path after input_dir
        fs::path out_path = output_dir / rel_path;
        out_path += ".spv"; // sprite.vert -> sprite.vert.spv

        // Recompile only if Source Shader is updated
        if (fs::exists(out_path) && fs::last_write_time(entry.path()) <= fs::last_write_time(out_path)) {
            continue;
        }

        if (compile_file(compiler, options, entry.path(), out_path)) {
            std::print("Entry: <{}>, Output <{}>, Status: OK\n", rel_path.string(), out_path.filename().string());
            ++compiled;
        }
        else {
            std::print("Entry: <{}>, Output <{}>, Status: FAILED\n", rel_path.string(), out_path.filename().string());
            ++failed;
        }
    }

    std::print("Compiling Done.\nCompiled: {}, Failed: {}\n", compiled, failed);
    return failed > 0 ? 1 : 0;
}