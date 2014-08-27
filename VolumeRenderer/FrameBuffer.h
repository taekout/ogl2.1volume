#pragma once
class FrameBuffer
{
public:
	FrameBuffer(void);
	~FrameBuffer(void);

	bool SetupRenderTarget(void);

protected:

	//std::map<int, int> fFramebuffers;
	//GLuint fDepthTex;
};

