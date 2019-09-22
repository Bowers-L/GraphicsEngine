 #include "Texture.h"

#include <stb_image/stb_image.h>

Texture::Texture(const std::string& type, const std::string& path)
	: m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	this->type = type;
	this->path = path;

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

	//These 4 parameters are required
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	//GLCall(glDeleteTextures(1, &m_TextureID));
}

void Texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::setParameter1i(GLenum parameterName, GLint parameter)
{
	glTexParameteri(GL_TEXTURE_2D, parameterName, parameter);
}
