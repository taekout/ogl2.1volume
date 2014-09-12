#include "FrameBuffer.h"
#include <GL/glew.h>



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
	glGenTextures(1, &fDepthTextureID);
	return true;
}

bool FrameBuffer::SetupRenderTarget(void)
{
	if(fFrameBufID == 0 || fDepthTextureID == 0)
		return false;
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	//glGenFramebuffers(1, &fID);
	glBindFramebuffer(GL_FRAMEBUFFER, fFrameBufID);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glBindTexture(GL_TEXTURE_2D, fDepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

