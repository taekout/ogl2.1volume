#pragma once

enum EDirection {
	left,
	right,
	forward,
	backward,
	up,
	down
};

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line);