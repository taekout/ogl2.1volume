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
	if(fFrameBufID != 0 || fDepthTextureID != 0)
		return false;

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	int width = 1024;
	int height = 1024;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // necessary?
	
	fFrameBufID = FramebufferName;
	fDepthTextureID = depthTexture;
	ImageTex * tex = new ImageTex(gRenderEngine->fTextureMgr->fTextures.size(), fDepthTextureID, NULL, width, height);
	gRenderEngine->fTextureMgr->fTextures.push_back(tex);
	
	return true;
}

bool FrameBuffer::SetupRenderTarget(void)
{
	if(fFrameBufID == 0 || fDepthTextureID == 0)
		return false;

	int width = 1024, height = 1024;
	glBindFramebuffer(GL_FRAMEBUFFER, fFrameBufID);
	glViewport(0, 0, width, height);
	return true;
}

void FrameBuffer::SetdownRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, gRenderEngine->WindowWidth(), gRenderEngine->WindowHeight());
}

