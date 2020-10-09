#include "CParticle.h"

CParticle::CParticle(glm::vec3 _pos, float _mass, float _damping)
{
	pos = _pos;
	mass = _mass;
	damping = _damping;

	moveable = true;
	force = glm::vec3(0.0f, 0.0f, 0.0f);
}

CParticle::~CParticle()
{
}

void CParticle::Move(float _deltaTime)
{
	// Check if the particle is moveable
	if (!moveable)
	{
		return;
	}
	else
	{
		pos += _deltaTime;
	}
}

void CParticle::Update(float _deltaTime)
{
	// Check if the particle is moveable
	if (!moveable)
	{
		return;
	}
	else
	{
		// Applying The Force
		glm::vec3 temp = pos;
		pos += (pos - prevPos) * damping + (force / mass) * (_deltaTime / 100);
		prevPos = temp;
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}