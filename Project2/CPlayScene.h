#pragma once
// Utilities
#include "CShaderLoader.h"
#include "CCamera.h"
#include "CInput.h"
#include "CTextLabel.h"

// Actors
#include "CActor.h"

// Meshes
#include "CPyramid.h"
#include "CSphere.h"
#include "CSkybox.h"
#include "CPlane.h"
#include "Model.h"
#include "CCube.h"

class CPlayScene
{
public:
	CPlayScene(CCamera* _gameCamera, CInput* _gameInput, FMOD::System* _audioSystem);
	~CPlayScene();

	void Render();
	void Update(GLfloat* deltaTime, ESceneManager* _currentScene);

private:
	void TextureGen(const char* textureLocation, GLuint* texture);

	bool isWireFrame = false;

	// Programs
	GLint program;
	GLint programSkybox;

	// Audio
	FMOD::System* audioSystem;

	// Utilities
	ESceneManager* currentScene;
	CCamera* gameCamera;
	CInput* gameInput;

	// Meshes
	CPyramid* actorPyramid;
	CCube* actorCube;
	CPlane* actorPlane;
	CCube* actorMeshTest;
	CSkybox* gameSkybox;

	// Objects
	CObject* actorCubeObj;
	CObject* actorCubeTwoObj;
	CObject* waterActor;
	CObject* quadObj;

	// Textures
	GLuint actorTex;
	GLuint actorWaterTex;
	GLuint actorCubeTex;

	// Text Labels
	CTextLabel* restartLabel;
};