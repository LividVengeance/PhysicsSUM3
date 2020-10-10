#include "CConstraints.h"

CConstraints::CConstraints(CParticle* _p1, CParticle* _p2)
{
	p1 = _p1;
	p2 = _p2;
	dist = glm::length(p1->pos - p2->pos);
}

CConstraints::~CConstraints()
{
}

void CConstraints::Satisfy()
{
	glm::vec3 v = p1->pos - p2->pos;
	v *= ((1.0f - dist / glm::length(v)) * 0.8f);
	p1->Move(-v);
	p2->Move(v);

}

void CConstraints::Render()
{
	glBegin(GL_LINES);
	glVertex3fv(glm::value_ptr(p1->pos));
	glVertex3fv(glm::value_ptr(p2->pos));
	glEnd();
}