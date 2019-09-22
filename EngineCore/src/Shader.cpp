#include "Shader.h"

#include "util/GLDebug.h"
#include "glad/glad.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_ShaderID(0)
{
	ShaderProgramSource source = parseShader(filepath);
	m_ShaderID = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ShaderID));
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		PRINTLN("Failed to compile shader");
		PRINTLN(message);
		return 0;
	}

	return id;
}

//retrieve the vertex source code and fragment source code from the file
ShaderProgramSource Shader::parseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {	//Getting a line from the stream and storing it in the line variable
		if (line.find("#shader") != std::string::npos) {	//std::string::npos is the equivalent of -1 (used to check for matches)
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_ShaderID));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int i)
{
	bind();
	GLCall(glUniform1i(getUniformLocation(name), i));
	unbind();
}

void Shader::setUniform1f(const std::string& name, float f)
{
	bind();
	GLCall(glUniform1f(getUniformLocation(name), f));
	unbind();
}

void Shader::setUniform3f(const std::string& name, float f0, float f1, float f2) {
	bind();
	GLCall(glUniform3f(getUniformLocation(name), f0, f1, f2));
	unbind();
}

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	bind();
	GLCall(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
	unbind();
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& mat4)
{
	bind();
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat4[0][0]));
	unbind();
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_ShaderID, name.c_str()));
	if (location == -1) {
		PRINTLN("Warning: Uniform " << name << " does not exist.");
	}
	return location;
}
