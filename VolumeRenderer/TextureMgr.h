#pragma once

#include <vector>

struct ImageTex
{
	unsigned int fActiveTexNo; // glActiveTexture ID.
	unsigned int fTexID; // GLTexture ID 
	unsigned char * fData;
	int fWidth;
	int fHeight;

	ImageTex(unsigned int activeTexNo, unsigned int glTexID, unsigned char * data, int width, int height) : fActiveTexNo(activeTexNo), fTexID(glTexID), fData(data), fWidth(width), fHeight(height) {}
};


class TextureMgr
{
public:
	TextureMgr(void);
	virtual ~TextureMgr(void);

	unsigned int CreateTexture(int width, int height, unsigned char * data);

	std::vector<ImageTex *> fTextures; // ID, texture data.
};
