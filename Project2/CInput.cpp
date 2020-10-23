#include "CInput.h"

CInput::CInput()
{
}

CInput::~CInput()
{
}

void CInput::ProcessInput()
{
	for (int i = 0; i < 255; i++)
	{
		if (KeyState[i] == INPUT_DOWN_FIRST)
		{
			KeyState[i] = INPUT_DOWN;
		}
		else if (KeyState[i] == INPUT_UP_FIRST)
		{
			KeyState[i] = INPUT_UP;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (MouseState[i] == INPUT_DOWN_FIRST)
		{
			MouseState[i] = INPUT_DOWN;
		}
		else if (MouseState[i] == INPUT_UP_FIRST)
		{
			MouseState[i] = INPUT_UP;
		}
	}
}

void CInput::MouseClick(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
	if (button >= 3)
	{
		return;
	}

	MouseState[button] = ((state == GLUT_DOWN) ? INPUT_DOWN_FIRST : INPUT_UP_FIRST);
}

void CInput::KeyboardDown(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_DOWN_FIRST;
}

void CInput::KeyboardUp(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_UP_FIRST;
}

void CInput::MousePassiveMove(int x, int y)
{
	//std::cout << "Passive x: " << x << " | y: " << y << std::endl;
	mouseX = (2.0f * x) / (float)Utils::SCR_WIDTH - 1.0f;
	mouseY = 1.0f - (2.0f * y) / (float)Utils::SCR_HEIGHT;
}

void CInput::MouseMove(int x, int y)
{
	//std::cout << "Clicked x: " << x << " | y: " << y << std::endl;
}

bool CInput::getKeyState(char key)
{
	// Gets KeyBoard State
	if (KeyState[key] == INPUT_DOWN)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

int CInput::getMouseX()
{
	return(mouseX);
}

int CInput::getMouseY()
{
	return(mouseY);
}

bool CInput::getClick(int button)
{
	if (MouseState[button] == INPUT_DOWN)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

bool CInput::GetFirstDown(int button)
{
	if (MouseState[button] == INPUT_DOWN_FIRST)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

bool CInput::UpdateMousePicking(CCamera* _camera, glm::vec3 _objPosition)
{
	// Screen pos
	glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);

	// Screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);
	
	// Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(_camera->CameraProjection());
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	// Eyespace to world space
	glm::mat4 invViewMat = glm::inverse(_camera->CameraView());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	rayDirection = glm::normalize(glm::vec3(rayWorld));

	// Intersection with object
	float radius = 2.0f;
	glm::vec3 v = _objPosition - _camera->GetCamPos();

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2 * glm::dot(v, rayDirection);
	float c = glm::dot(v, v) - radius * radius;
	float d = b * b - 4 * a * c;

	float x1 = (-b - sqrt(d)) / 2;
	float x2 = (-b + sqrt(d)) / 2;
	if ((d > 0) && (x1 >= 0 && x2 >= 0) || (x1 < 0 && x2 >= 0))
	{
		return(true);
	}
	return(false);
}