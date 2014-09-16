#include "TextureMgr.h"
#include <GL/glew.h>

TextureMgr::TextureMgr(void)
{
}


TextureMgr::~TextureMgr(void)
{
}


unsigned int TextureMgr::CreateTexture(int width, int height, unsigned char * data)
{
	// Create one OpenGL texture
	
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	if(fTextures.find(textureID) == fTextures.end()) {
		ImageTex *tex = new ImageTex(textureID, data, width, height);
		fTextures.insert(std::pair<unsigned int, ImageTex *>(textureID, tex));
		return textureID;
	}

	throw "Texture ID already generated.";

	return textureID;
}

