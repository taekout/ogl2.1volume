#pragma once

#include <map>

struct ImageTex
{
	unsigned int fID;
	unsigned char * fData;
	int fWidth;
	int fHeight;

	ImageTex(unsigned int ID, unsigned char * data, int width, int height) : fID(ID), fData(data), fWidth(width), fHeight(height) {}
};


class TextureMgr
{
public:
	TextureMgr(void);
	virtual ~TextureMgr(void);

	unsigned int CreateTexture(int width, int height, unsigned char * data);

protected:
	std::map<unsigned int, ImageTex *> fTextures; // ID, texture data.
};
