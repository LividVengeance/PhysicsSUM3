#include "CCloth.h"

CCloth::CCloth(float _clothWidth, float _clothHeight, int _particleWidth, 
	int _particleHeight, float _mass, float _damping, glm::mat4 _modelMatrix, GLuint* _texture)
{
	clothHeight = _clothHeight;
	clothWidth = _clothWidth;
	particleHeight = _particleHeight;
	particleWidth = _particleWidth;
	mass = _mass;
	damping = _damping;
	modelMatrix = _modelMatrix;
	texture = _texture;

	// Particle Var Setup
	float distPartX = clothWidth / particleWidth;
	float distPartY = clothHeight/ particleHeight;
	float numPart = particleWidth * particleHeight;
	float partMass = numPart / mass;
	float partDamping = 1 - damping;

	// Creates all Particles in Cloth
	for (int i = 0; i < particleHeight; i++)
	{
		for (int j = 0; j < particleWidth; j++)
		{
			glm::vec3 partPos = (glm::vec3)(modelMatrix * glm::vec4(i * distPartX, -j * distPartY, 0, 1));
			allPartsInCloth.push_back(CParticle(partPos, partMass, partDamping));
		}
	}

	// Creates the Triangles between the Particels on the cloth
	for (int i = 0; i < particleHeight; i++)
	{
		for (int j = 0; j < particleWidth; j++)
		{
			CParticle* p1 = &allPartsInCloth[(i - 1) * particleWidth + j - 1];
			CParticle* p2 = &allPartsInCloth[(i - 1) * particleWidth + j];
			CParticle* p3 = &allPartsInCloth[i * particleWidth + j - 1];
			CParticle* p4 = &allPartsInCloth[i* particleWidth + j];

			triangles.push_back(CTriangle(p3, p1, p2));
			triangles.push_back(CTriangle(p2, p4, p3));

		}
	}

	allPartsInCloth[numPart / 2 + particleWidth / 2].force.z -= 1;

	// Pins the Top Left and Right - To Keep Cloth Up
	allPartsInCloth[0].moveable = false;
	allPartsInCloth[particleWidth - 1].moveable = false;
}

CCloth::~CCloth()
{
}

void CCloth::Update()
{

}

void CCloth::Render()
{

}