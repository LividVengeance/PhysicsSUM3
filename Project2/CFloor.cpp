#include "CFloor.h"

CFloor::CFloor(glm::vec3 _pos, float _width, float _depth)
{
	pos = _pos;
	width = _width;
	depth = _depth;
}

CFloor::~CFloor()
{
}

void CFloor::Render()
{

}

void CFloor::Update(std::vector<CParticle*> allPartsInCloth)
{
	for (std::vector<CParticle>::size_type i = 0; i < allPartsInCloth.size(); i++)
	{
		glm::vec3 partPos = (allPartsInCloth)[i]->pos;
		if (partPos.y <= pos.y)
		{
			(allPartsInCloth)[i]->pos += glm::vec3(0, pos.y - partPos.y, 0);
		}
	}
}