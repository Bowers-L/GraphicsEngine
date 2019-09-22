#pragma once

#include <glm/mat4x4.hpp>

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	const std::string& m_FilePath;
	unsigned int m_ShaderID;
	std::unordered_map<std::string, int> m_UniformLocationCache;	//Used for optimization so that the uniform location does not have to be retrieved every time
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	//uniforms
	void setUniform1i(const std::string& name, int i);
	void setUniform1f(const std::string& name, float f);
	void setUniform3f(const std::string& name, float f0, float f1, float f2);
	void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void setUniformMat4f(const std::string& name, const glm::mat4& mat4);
private:
	unsigned int createShader(const std::string& vertextShader, const std::string& fragmentShader);
	unsigned int compileShader(unsigned int type, const std::string& source);
	ShaderProgramSource parseShader(const std::string& filepath);
	
	int getUniformLocation(const std::string& name);
};
