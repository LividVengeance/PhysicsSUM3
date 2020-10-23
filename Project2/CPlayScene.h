#pragma once
// Utilities
#include "CShaderLoader.h"
#include "CCamera.h"
#include "CInput.h"
#include "CTextLabel.h"

// Actors
#include "CActor.h"

// Physics
#include "CCloth.h"

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
	bool Button(float width, float height, CObject* _buttonObj);
	void ButtonChecks();
	void InitButtons();
	bool isWireFrame = false;

	// Programs
	GLint program;
	GLint programNoFog;
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

	CParticle* partTest;

	CCloth* clothSim;

	// Objects
	CObject* actorCubeObj;

	// Textures
	GLuint actorTex;
	GLuint resetTex;
	GLuint dropTex;
	GLuint increasePinTex;
	GLuint decreasePinTex;
	GLuint windrightTex;
	GLuint windleftTex;
	GLuint applyWindTex;

	// Text Labels
	CTextLabel* restartLabel;

	// Buttons
	CPlane* dropPlane;
	CPlane* increasePlane;
	CObject* dropObj;
	CObject* windLeftObj;
	CObject* windRightObj;
	CObject* resetObj;
	CObject* increasePinObj;
	CObject* decreasePinObj;
	CObject* applyWindObj;

	float mouseX;
	float mouseY;
	int particleHeight;
};