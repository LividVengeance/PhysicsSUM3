#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "CParticle.h"
#include <vector>

class CFloor
{
public:
	CFloor(glm::vec3 _pos, float _width, float _depth);
	~CFloor();
	void Render();
	void Update(std::vector<CParticle*> allPartsInCloth);

private:
	glm::vec3 pos;
	float width;
	float depth;
};

