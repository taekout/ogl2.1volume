#pragma once
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

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


class UserInput 
{
protected:
	bool bShiftDown;
	int lastX, lastY, buttonDown;

	MouseHistory * mouseHistory;

	double angleX, angleY, angleZ;
	double positionX, positionY;
	double positionZ;
	//double distanceFromObj;

public:
	UserInput();
	void Keyboard(unsigned char key, int x, int y);
	void Mouse(int button, int state, int x, int y);
	void MouseMotion(int x, int y);
	~UserInput(void);

	// UpdateGlobalRotationAngle
	void UpdateGlobalRotation(void);

	//Get Set functions
	inline double GetAngleX() {
		return angleX;
	}
	inline double GetAngleY() {
		return angleY;
	}
	inline double GetAngleZ() {
		return angleZ;
	}
	/*inline double GetViewDistance() {
		return distanceFromObj;
	}*/

};

