#pragma once
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "glut.h"


class ShaderFileTimeStampDiff
{
	__time64_t prevShaderFileTimeStamp;
	__time64_t shaderFileTimeStamp;
public:
	ShaderFileTimeStampDiff() {
		prevShaderFileTimeStamp = -1;
		shaderFileTimeStamp = -1;
	}
	ShaderFileTimeStampDiff(__time64_t prev, __time64_t current) {
		prevShaderFileTimeStamp = prev;
		shaderFileTimeStamp = current;
	}

	inline __time64_t GetCurTime() {
		return shaderFileTimeStamp;
	}
	bool IsChanged()
	{
		return false;
#if 0
		return prevShaderFileTimeStamp != shaderFileTimeStamp;
#endif
	}

	ShaderFileTimeStampDiff & operator = (__time64_t newTimeStamp)
	{
		prevShaderFileTimeStamp = shaderFileTimeStamp;
		shaderFileTimeStamp = newTimeStamp;
		return *this;
	}
};

class Shader
{
	ShaderFileTimeStampDiff vertTimeStamp;
	ShaderFileTimeStampDiff fragTimeStamp;

protected:
	char *textFileRead(char *fn);
	std::string *textProcess(std::string data);

public:

	enum EShaderKind {
		eShaderNothing = -1,
		eShaderBasic = 0,
		eShaderTexture = 1,
		eShaderShadowMapGeneration = 2,
		eShaderBasicWithShadow = 3,
		eMaxShader
	};

	struct ShaderData {
		ShaderData(EShaderKind kind, GLuint vertID, GLuint fragID, GLuint programID, std::string & vertFile, std::string & fragName)
			: fKind(kind), fVertShaderID(vertID), fFragShaderID(fragID), fProgramID(programID), vertFilename(vertFile), fragFilename(fragName)
			, fVertexID(-1), fNormalID(-1), fUVID(-1)
		{}
		EShaderKind fKind;
		GLuint fVertShaderID;
		GLuint fFragShaderID;
		GLuint fProgramID;

		int fVertexID;
		int fNormalID;
		int fUVID;

		std::string vertFilename;
		std::string fragFilename;
	};


	Shader(void);
	~Shader(void);

	virtual void setShaders(EShaderKind kind, char * vertShader, char *fragShader);
	virtual void CompileAllShaders();
	virtual void LinkShaders();

	virtual void UseProgram(EShaderKind kind);

	int textFileWrite(char *fn, char *s);
	std::string *ShaderFileRead(std::string filename, std::string shaderKind);
	void ShaderFileChangeWatcher(void);
	// Get Functions
	GLuint GetProgram(void) { return (fShaderIndex != -1) ? fShaderData[fShaderIndex].fProgramID : 0; }
	GLuint GetProgram(EShaderKind kind) { return fShaderData[kind].fProgramID; }
	ShaderData GetShaderData(EShaderKind kind);
	// Log Functions
	void printShaderInfoLog(std::string shadername, GLuint obj);
	void printProgramInfoLog(std::string vertShaderName, std::string fragShaderName, GLuint obj);

	// Update GL Variable Functions
	void UpdateUniform1i(char *varName, unsigned int data);
	void UpdateUniform4fv(char *varName, float data1, float data2, float data3, float data4);
	void UpdateUniform1f(char *varName, float data);
	void UpdateUniform3fv(char *varName, float data1, float data2, float data3);
	void UpdateUniformMat4(char *varName, float * data);

private:
	
	std::vector<ShaderData> fShaderData;
	int fShaderIndex;
	
};