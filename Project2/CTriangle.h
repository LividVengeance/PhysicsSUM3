#pragma once

#include "CParticle.h"

class CTriangle
{
public:
	CTriangle(CParticle* p1, CParticle* p2, CParticle* p3);
	~CTriangle();

	void Render();

	CParticle* particle[3];
	
private:

};

