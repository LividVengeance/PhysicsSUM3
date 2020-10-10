#pragma once
#include "CParticle.h"

class CConstraints
{
public:
	CConstraints(CParticle* _p1, CParticle* _p2);
	~CConstraints();

	void Satisfy();
	void Render();

private:
	CParticle* p1;
	CParticle* p2;
	float dist;
};
