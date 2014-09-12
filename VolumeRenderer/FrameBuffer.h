#pragma once
class FrameBuffer
{
public:
	FrameBuffer(void);
	~FrameBuffer(void);

	bool CreateFrameBuffer(void);
	bool SetupRenderTarget(void);
	void SetdownRenderTarget(void);
	unsigned int DepthTexID() { return fDepthTextureID; }

protected:
	unsigned int fFrameBufID;
	unsigned int fDepthTextureID;
	//std::map<int, int> fFramebuffers;
	//GLuint fDepthTex;
};

