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

#define forit( container )	for(auto i = container.begin() ; i != container.end() ; ++i)
#define fori( array, size )	for(size_t i = 0 ; i < size ; i++)

unsigned int loadBMP_custom(const char * imagepath);



