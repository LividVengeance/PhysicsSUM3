#include "CParticle.h"

CParticle::CParticle(glm::vec3 _pos, float _mass, float _damping, CCube* _partMesh, 
	GLuint*_program, GLuint* _texture, CCamera* _gameCamera, CInput* _gameInput)
{
	pos = _pos;
	prevPos = pos;
	mass = _mass;
	damping = _damping;
	program = _program;
	partMesh = _partMesh;
	texture = _texture;
	gameCamera = _gameCamera;
	gameInput = _gameInput;

	isFrozen = false;
	force = glm::vec3(0.0f, 0.0f, 0.0f);
	objAngleRotation = 0.0f;
	objRotaion = glm::vec3(0.0f, 1.0f, 0.0f);
	objScale = glm::vec3(1.0f, 1.0f, 1.0f);
	objScaleAmount = 1.0f;
}

CParticle::~CParticle()
{
}

void CParticle::Move(glm::vec3 _delta)
{
	// Check if the particle is moveable
	if (!isFrozen)
	{
		pos += _delta;
	}
}

void CParticle::Update(float _deltaTime)
{
	//force = glm::vec3(0.0f, 0.0f, 0.0f);
	//Check if the particle is moveable
	if (!isFrozen)
	{
		// Applying The Force
		glm::vec3 temp = pos;
		pos += (pos - prevPos) * damping + (force / mass) * (_deltaTime * 1000.0f);
		prevPos = temp;
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	glm::mat4 translationMatrix = glm::translate(glm::mat4(), pos);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(objAngleRotation), objRotaion);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), objScale * objScaleAmount);

	// Create model matrix to combine them
	objModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	// Mouse on the particle
	if (gameInput->getClick(0) || gameInput->getClick(1))
	{
		if (gameInput->UpdateMousePicking(gameCamera, pos))
		{
			std::cout << "Mouse over particle" << std::endl;
		}
	}
}

void CParticle::Render()
{
	glUseProgram(*program);
	glBindVertexArray(*partMesh->GetVAO());		// Bind VAO
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glUniform1i(glGetUniformLocation(*program, "tex"), 0);

	GLuint modelLoc = glGetUniformLocation(*program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(objModelMatrix));

	GLuint projection = glGetUniformLocation(*program, "proj");
	glUniformMatrix4fv(projection, 1, GL_FALSE, value_ptr(gameCamera->CameraProjection()));

	GLuint view = glGetUniformLocation(*program, "view");
	glUniformMatrix4fv(view, 1, GL_FALSE, value_ptr(gameCamera->CameraView()));

	GLuint camPos = glGetUniformLocation(*program, "camPos");
	glUniform3fv(camPos, 1, value_ptr(gameCamera->GetCamPos()));

	glDrawElements(GL_TRIANGLES, partMesh->GetIndiceCount(), GL_UNSIGNED_INT, 0); // Drawing Background
	glBindVertexArray(0);
	glUseProgram(0);
}