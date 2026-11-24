#pragma once
#include "Rendering/RHI/ShaderHandle.h"

namespace Engine::Graphics {
	class GL_Shader : public ShaderHandle {
	public:
		GL_Shader(
			const std::filesystem::path& vertex_shader_path,
			const std::filesystem::path& fragment_shader_path,
			std::error_code& ec
		);
		~GL_Shader();
		void uninit() override;

		void use_shader() const;

		int32_t get_uniform_location(const char* name) const;
		void set_uniform_float(const void* data, int32_t uniform_location) const;
		void set_uniform_vec2(const void* data, int32_t uniform_location) const;
		void set_uniform_vec3(const void* data, int32_t uniform_location) const;
		void set_uniform_vec4(const void* data, int32_t uniform_location) const;
	private:
		int32_t id = 0;
	};
}