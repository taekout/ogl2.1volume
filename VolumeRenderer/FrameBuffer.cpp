#include "FrameBuffer.h"
#include "RenderEngine.h"
#include "TextureMgr.h"
#include <GL/glew.h>

extern RenderEngine * gRenderEngine;

FrameBuffer::FrameBuffer(void)
{
	fFrameBufID = 0;
	fDepthTextureID = 0;
}


FrameBuffer::~FrameBuffer(void)
{
}

bool FrameBuffer::CreateFrameBuffer()
{
	glGenFramebuffers(1, &fFrameBufID);
	
	return true;
}

bool FrameBuffer::SetupRenderTarget(void)
{
	if(fFrameBufID == 0)
		return false;
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenTextures(1, &fDepthTextureID);
	glBindFramebuffer(GL_FRAMEBUFFER, fFrameBufID);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader

	int width = 1024, height = 1024;
	glBindTexture(GL_TEXTURE_2D, fDepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE0 + 0);//gRenderEngine->fTextureMgr->fTextures.size());
	ImageTex * tex = new ImageTex(gRenderEngine->fTextureMgr->fTextures.size(), fDepthTextureID, NULL, width, height);
	gRenderEngine->fTextureMgr->fTextures.push_back(tex);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, fDepthTextureID, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	return true;
}

void FrameBuffer::SetdownRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

