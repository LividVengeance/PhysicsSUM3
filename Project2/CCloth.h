#pragma once

// Utilites
#include "CCamera.h"
#include "CShaderLoader.h"
#include "CInput.h"
#include "CFloor.h"

// Physics
#include "CConstraints.h"
#include "CParticle.h"

// Mesh
#include "CCube.h"

#include <vector>
#include <iostream>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class CCloth
{
public:
	CCloth(float _clothWidth, float _clothHeight, int _particleWidth,
		int _particleHeight, float _mass, float _damping, glm::vec3 _clothPos,
		CCamera* _gameCamera, CInput* _gameInputs);
	~CCloth();
	
	void Update(float _deltaTime);
	void Render();
	std::vector<CParticle*> allPartsInCloth;
	int particleWidth;
	int wind = 0;
	bool apply = false;

private:
	void TextureGen(const char* textureLocation, GLuint *texture);
	void PinUpdates();
	void ResetCloth();

	float clothWidth;
	float clothHeight;
	
	int particleHeight;
	float mass;
	float damping;

	int currentSelected;

	std::vector<CConstraints*> allConsnInCloth;

	glm::vec3 clothPos;
	GLuint texture;
	GLuint selectedTex;
	GLuint program;
	CCube* partMesh;
	CCamera* gameCamera;
	CInput* gameInput;
	CFloor* floor;
};

