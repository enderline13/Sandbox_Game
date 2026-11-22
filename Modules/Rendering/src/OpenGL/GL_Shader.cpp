#include "Rendering/OpenGL/GL_Shader.h"
#include <glad/gl.h>
#include <Utility/Logger.h>
#include <Base/EngineError.h>

Engine::Graphics::GL_Shader::GL_Shader(
	const std::filesystem::path& vertex_shader_path,
	const std::filesystem::path& fragment_shader_path,
	std::error_code& ec
) {
	std::string vertexContents = read_file_string(vertex_shader_path, ec);
	if (ec) return;
	std::string fragmentContetns = read_file_string(fragment_shader_path, ec);
	if (ec) return;

	const char* vertexSource = vertexContents.c_str();
	const char* fragmentSource = fragmentContetns.c_str();

	auto compileShader = [](GLenum type, const char* src, GLuint& shader_id, std::error_code& ec) {
		shader_id = glCreateShader(type);
		glShaderSource(shader_id, 1, &src, nullptr);
		glCompileShader(shader_id);

		GLint success = 0;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint length = 0;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

			std::string info(length, '\0');
			glGetShaderInfoLog(shader_id, length, nullptr, info.data());

			std::string shader_error = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
			shader_error += " SHADER ERROR:\n";
			shader_error += info;
			Engine::Util::Logger::Log(shader_error);
			ec = Engine::Errors::EngineError::OpenGLShaderCreateError;
		}
	};

	GLuint vertexShader{}, fragmentShader{};
	if (!ec) compileShader(GL_VERTEX_SHADER, vertexSource, vertexShader, ec);
	if (!ec) compileShader(GL_FRAGMENT_SHADER, fragmentSource, fragmentShader, ec);
	if (ec) return;

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		GLint length = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

		std::string info(length, '\0');
		glGetProgramInfoLog(id, length, nullptr, info.data());

		std::string shader_error = "PROGRAM LINK ERROR:\n";
		shader_error += info;
		Engine::Util::Logger::Log(shader_error);
		ec = Engine::Errors::EngineError::OpenGLShaderCreateError;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Engine::Graphics::GL_Shader::~GL_Shader() {
	uninit();
}

void Engine::Graphics::GL_Shader::uninit() {
	if (id) {
		glDeleteProgram(id);
		id = 0;
	}
}

void Engine::Graphics::GL_Shader::use_shader() const {
	glUseProgram(id);
}