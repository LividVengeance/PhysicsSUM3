#pragma once
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <iostream>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Utilities.h"
#include "CCamera.h"


class CInput
{
public:
	CInput();
	~CInput();

	void ProcessInput();
	bool getKeyState(char key);

	// KeyBoard Inputs
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

	// Mosue Inputs
	void MousePassiveMove(int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	bool UpdateMousePicking(CCamera* _camera, glm::vec3 _objPosition);

	int getMouseX();
	int getMouseY();
	bool getClick(int button);

	bool GetFirstDown(int button);

private:
	InputState KeyState[255];
	InputState MouseState[3];

	int mouseX = 0;
	int mouseY = 0;
	glm::vec3 rayDirection;
};