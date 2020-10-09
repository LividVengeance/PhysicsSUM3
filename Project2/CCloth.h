#pragma once
#include "CCamera.h"
#include "CParticle.h"
#include "CTriangle.h"

#include <vector>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class CCloth
{
public:
	CCloth(float _clothWidth, float _clothHeight, int _particleWidth,
		int _particleHeight, float _mass, float _damping, glm::mat4 _modelMatrix, GLuint* _texture);
	~CCloth();

	void Update();
	void Render();

private:
	float clothWidth;
	float clothHeight;
	int particleWidth;
	int particleHeight;
	float mass;
	float damping;

	std::vector<CParticle> allPartsInCloth;
	std::vector<CTriangle> triangles;

	glm::mat4 modelMatrix;
	GLuint* texture;
};

