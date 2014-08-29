#pragma once
class FrameBuffer
{
public:
	FrameBuffer(void);
	~FrameBuffer(void);

	bool SetupRenderTarget(void);
	void SetdownRenderTarget(void);

protected:

	//std::map<int, int> fFramebuffers;
	//GLuint fDepthTex;
};

