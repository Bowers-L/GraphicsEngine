#pragma once

#include <glad/glad.h>
#include "util/GLDebug.h"
#include "glm/glm.hpp"

class Texture {
private:
	unsigned int m_TextureID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; //Bits per pixel
public:
	std::string type;
	std::string path;

	Texture(const std::string& type, const std::string& path);
	~Texture();

	void bind(unsigned int slot) const;
	void unbind() const;

	void setParameter1i(GLenum parameterName, GLint parameter);

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
};