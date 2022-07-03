#pragma once

class Texture
{
public:
	Texture(const char* filepath);
	~Texture();

	void Bind();
	void Unbind();

	int m_Width, m_Height, m_BPP;

private:
	unsigned char* m_LocalBuffer;
};