#pragma once

// Utilites
#include "CCamera.h"
#include "CShaderLoader.h"

// Physics
#include "CConstraints.h"
#include "CParticle.h"

// Mesh
#include "CCube.h"

#include <vector>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class CCloth
{
public:
	CCloth(float _clothWidth, float _clothHeight, int _particleWidth,
		int _particleHeight, float _mass, float _damping, glm::mat4 _modelMatrix, 
		CCamera* _gameCamera);
	~CCloth();

	void Update(float _deltaTime);
	void Render();

private:
	void TextureGen(const char* textureLocation, GLuint* texture);

	float clothWidth;
	float clothHeight;
	int particleWidth;
	int particleHeight;
	float mass;
	float damping;

	std::vector<CParticle> allPartsInCloth;
	std::vector<CConstraints> allConsnInCloth;

	glm::mat4 modelMatrix;
	GLuint* texture;
	GLint program;
	CCube* partMesh;
	CCamera* gameCamera;
};

