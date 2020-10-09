#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class CParticle

{
public:
	CParticle(glm::vec3 _pos, float _mass, float _damping);
	~CParticle();

	void Move(float _deltaTime);
	void Update(float _deltaTime);
	
	bool moveable;
	glm::vec3 force;

private:
	glm::vec3 pos;
	glm::vec3 prevPos;
	float mass;
	float damping;
};

