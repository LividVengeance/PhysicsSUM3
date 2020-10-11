#pragma once
#include "CCube.h"
#include "CCamera.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

class CParticle

{
public:
	CParticle(glm::vec3 _pos, float _mass, float _damping, CCube* _partMesh, 
		GLuint* _program, GLuint* texture, CCamera* _gameCamera);
	~CParticle();

	void Move(glm::vec3 _delta);
	void Update(float _deltaTime);
	void Render();
	
	bool moveable;
	glm::vec3 force;
	glm::vec3 pos;

private:
	
	glm::vec3 prevPos;
	float mass;
	float damping;

	// Object Location
	glm::vec3 objPosition;
	glm::vec3 objScale;
	glm::vec3 objRotaion;
	float objAngleRotation;
	float objScaleAmount;
	glm::mat4 objModelMatrix;

	CCube* partMesh;
	GLuint* program;
	GLuint* texture;
	CCamera* gameCamera;
};

