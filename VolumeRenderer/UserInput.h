#pragma once
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class MouseHistory
{
	double globalAngleX, globalAngleY;
	static const int MOUSE_HISTORY_SIZE = 10;
	int * historyX;
	int * historyY;
	time_t *timestampHistory;
	int currentIndexX, currentIndexY;
public:
	MouseHistory();
	~MouseHistory();

	void InitializeHistory();
	float MouseMotionVelocityX();
	float MouseMotionVelocityY();
	void SetGlobalAngles();

	// Get Func
	inline double GetGlobalAngleX() {
		return globalAngleX;
	}
	inline double GetGlobalAngleY() {
		return globalAngleY;
	}

public:
	void NewHistory(int x, int y) {
		if(currentIndexX == MOUSE_HISTORY_SIZE - 1) currentIndexX = -1;
		if(currentIndexY == MOUSE_HISTORY_SIZE - 1) currentIndexY = -1;
		historyX[++currentIndexX] = x;
		historyY[++currentIndexY] = y;
		time(&timestampHistory[currentIndexX]);
	}
};

class Camera;

class UserInput 
{
protected:
	MouseHistory * fMouseHistory;
	Camera * fCamera;

public:
	UserInput(Camera * curCamera);
	void Keyboard(unsigned char key, int x, int y);
	void Keyboard(int key, int x, int y);
	void Mouse(int button, int state, int x, int y);
	void MouseMotion(int x, int y);
	~UserInput(void);

protected:
	glm::vec2 fMousePos;
	int fButtonPressed;
	glm::vec2 fAngles;

};

