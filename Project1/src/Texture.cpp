#include "Texture.h"
#include "vendor/stb_image/stb_image.h"


Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{

	stbi_set_flip_vertically_on_load(1); // flip the image vertically
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 4 channels for RGBA

	GLCall(glGenTextures(1, &m_RendererID)); // generate a texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // bind the texture

	// parameters for the texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer)); // create the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind the texture
	if (m_LocalBuffer) // if the local buffer is not null
		stbi_image_free(m_LocalBuffer); // free the local buffer
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID)); // delete the texture
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // activate the texture slot
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // bind the texture
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind the texture cuz 0 means unbind
}

