#include "UserInput.h"
#include "glut.h"

UserInput::UserInput()
	: lastX(-1), lastY(-1), buttonDown(-1), angleX(0.0), angleY(0.0), angleZ(0.0)
	, positionX(0.0f), positionY(0.0f), positionZ(-4.0f), bShiftDown(false)//, globalAngleX(0.0), globalAngleY(0.0)
{
	mouseHistory = new MouseHistory;
	//distanceFromObj = viewPosition[0] * viewPosition[0] + viewPosition[1] * viewPosition[1] + viewPosition[2] * viewPosition[2];
	//distanceFromObj = sqrt(distanceFromObj);
}

UserInput::~UserInput(void)
{
	if(mouseHistory)
		delete mouseHistory;
}

void UserInput::UpdateGlobalRotation(void){
	angleY -= mouseHistory->GetGlobalAngleX() * 0.1;
	angleX -= mouseHistory->GetGlobalAngleY() * 0.1;
}

void UserInput::Mouse(int button, int state, int x, int y)
{
	// if it is when users let go of the left button while motioning, then objects should keep rotating.
    // Record the mouse position when a button is pressed.
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		// could Use Visitor pattern.
		// check the prev history and get the velocity of mouse cursor motion/move.
		mouseHistory->SetGlobalAngles();
	}
	else {
		mouseHistory->InitializeHistory();
	}
    lastX = x;
    lastY = y;
    buttonDown = button;
    bShiftDown = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
}

void UserInput::MouseMotion(int x, int y)
{
    int deltaX = x - lastX;
    int deltaY = y - lastY;

    switch(buttonDown) {
    case GLUT_LEFT_BUTTON:                  // Rotate in the x-y plane
        angleX -= deltaY;
        angleY -= deltaX;
		mouseHistory->NewHistory(x, y);
        break;
    case GLUT_RIGHT_BUTTON:                 // Rotate in the x-z plane
//        angleX -= deltaY;
//        angleZ -= deltaX;
		//distanceFromObj -= deltaY * 0.1;
        break;
    case GLUT_MIDDLE_BUTTON:
        if (bShiftDown) {                  // Move in and out
            positionZ -= deltaY * 0.1;
        } else {                            // Move left and right
            positionX -= deltaX * 0.1;
            positionY += deltaY * 0.1;
        }
        break;
    }
    lastX = x;
    lastY = y;
}

void UserInput::Keyboard(unsigned char key, int x, int y)
{
	// Exit on escape.
	switch(key) 
	{
	case 27:
		exit(0);
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_UP:
		break;
	default:
		std::cout << "unknown key " << key << std::endl;
		break;
	}
}

float MouseHistory::MouseMotionVelocityX()
{
	// Warning! Time stamp didn't really work! It's because in a second, there is no time difference in time_t data structure.
	// I guess millisecond unit might work. but I am not sure how to do it yet.
	int differenceBtwCurrentAndLastIndex = 0;
	for(int i = 0 ; i < MOUSE_HISTORY_SIZE ; i++) {
		int index = (currentIndexX - i < 0) ? currentIndexX - i + MOUSE_HISTORY_SIZE : currentIndexX - i;
		if(historyX[index] == -100) {
			differenceBtwCurrentAndLastIndex = i;
			break;
		}
	}
	float velocityX;
	if(differenceBtwCurrentAndLastIndex == 0)
		return 0.0f;
	if(currentIndexX - differenceBtwCurrentAndLastIndex + 1 < 0)
		velocityX = float(historyX[currentIndexX] - historyX[currentIndexX - differenceBtwCurrentAndLastIndex + 1 + MOUSE_HISTORY_SIZE]) / (differenceBtwCurrentAndLastIndex + 1);
	else
		velocityX = float(historyX[currentIndexX] - historyX[currentIndexX - differenceBtwCurrentAndLastIndex + 1]) / (differenceBtwCurrentAndLastIndex + 1);
	return velocityX;
	/*
	int tmpX = (currentIndexX - 9) % MOUSE_HISTORY_SIZE;
	if(tmpX < 0) tmpX += this ->MOUSE_HISTORY_SIZE;
	float velocityX;
	if(historyX[tmpX] == -100)
		return 0.0f;
	velocityX = historyX[currentIndexX] - historyX[tmpX];
	return velocityX;
	*/
}

void MouseHistory::SetGlobalAngles()
{
	globalAngleX = MouseMotionVelocityX(); globalAngleY = MouseMotionVelocityY();
}


MouseHistory::MouseHistory()
{
	historyX = new int[MOUSE_HISTORY_SIZE];
	historyY = new int[MOUSE_HISTORY_SIZE];
	timestampHistory = new time_t[MOUSE_HISTORY_SIZE];
	for(int i = 0 ; i < MOUSE_HISTORY_SIZE ; i++) {
		historyX[i] = -100;
		historyY[i] = -100;
		timestampHistory[i] = -1;
	}
	currentIndexX = -1;
	currentIndexY = -1;
	globalAngleX = globalAngleY = 0.0;
}

MouseHistory::~MouseHistory() {
	delete [] historyX;
	delete [] historyY;
}

void MouseHistory::InitializeHistory()
{
	for(int i = 0 ; i < MOUSE_HISTORY_SIZE ; i++) {
		historyX[i] = -100;
		historyY[i] = -100;
	}
	currentIndexX = -1;
	currentIndexY = -1;
	globalAngleX = globalAngleY = 0.0;
}

float MouseHistory::MouseMotionVelocityY()
{
	// Warning! Time stamp didn't really work! It's because in a second, there is no time difference in time_t data structure.
	// I guess millisecond unit might work. but I am not sure how to do it yet.
	int differenceBtwCurrentAndLastIndex = 0;
	for(int i = 0 ; i < MOUSE_HISTORY_SIZE ; i++) {
		int index = (currentIndexX - i < 0) ? currentIndexY - i + MOUSE_HISTORY_SIZE : currentIndexY - i;
		if(historyY[index] == -100) {
			differenceBtwCurrentAndLastIndex = i;
			break;
		}
	}
	float velocityY;
	if(differenceBtwCurrentAndLastIndex == 0)
		return 0.0f;
	if(currentIndexY - differenceBtwCurrentAndLastIndex + 1 < 0)
		velocityY = float(historyY[currentIndexY] - historyY[currentIndexY - differenceBtwCurrentAndLastIndex + 1 + MOUSE_HISTORY_SIZE]) / (differenceBtwCurrentAndLastIndex + 1);
	else
		velocityY = float(historyY[currentIndexY] - historyY[currentIndexY - differenceBtwCurrentAndLastIndex + 1]) / (differenceBtwCurrentAndLastIndex + 1);
	return velocityY;
}