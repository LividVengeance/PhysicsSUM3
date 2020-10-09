#include "CTriangle.h"

CTriangle::CTriangle(CParticle* p1, CParticle* p2, CParticle* p3)
{
	particle[0] = p1;
	particle[1] = p2;
	particle[2] = p3;

}

CTriangle::~CTriangle()
{
}

void CTriangle::Render()
{
	for (int i = 0; i < 3; i++)
	{

	}
}