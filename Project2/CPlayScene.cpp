#include "CPlayScene.h"

CPlayScene::CPlayScene(CCamera* _gameCamera, CInput* _gameInput, FMOD::System* _audioSystem)
{
	gameCamera = _gameCamera;
	gameInput = _gameInput;
	audioSystem = _audioSystem;

	// Creates Programs For Objects In Scene
	program = CShaderLoader::CreateProgram("Resources/Shaders/Fog.vs",
		"Resources/Shaders/Fog.fs");

	programNoFog = CShaderLoader::CreateProgram("Resources/Shaders/Basic.vs",
		"Resources/Shaders/Basic.fs");

	programSkybox = CShaderLoader::CreateProgram("Resources/Shaders/skybox.vs",
		"Resources/Shaders/skybox.fs");

	// Generate Texte
	const char* fileLocation = "Resources/Textures/BackgroundSprite.png";
	TextureGen(fileLocation, &actorTex);

	const char* cubeFileLocation = "Resources/Textures/frogChair.jpg";
	TextureGen(cubeFileLocation, &actorCubeTex);

	const char* waterFileLocation = "Resources/Textures/WaterSprite.png";
	TextureGen(waterFileLocation, &actorWaterTex);

	// Creates Mesh
	actorPyramid = new CPyramid();
	actorCube = new CCube(0.1f);

	//partTest = new CParticle(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, -2.0f, actorCube, &programNoFog, &actorTex, gameCamera);

	// Create Game Actors
	actorCubeObj = new CObject(&programNoFog, actorCube->GetVAO(), actorCube->GetIndiceCount(), gameCamera, &actorTex);


	clothSim = new CCloth(10.0f, 5.0f, 20.0f, 10.0f, 100.0f, 0.01, glm::vec3(0.0f, 0.0f, 0.0f), gameCamera);

	// Create Skybox
	gameSkybox = new CSkybox(&programSkybox, gameCamera);

	// Labels
	restartLabel = new CTextLabel("Press 'R' to restart", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 570.0f), glm::vec3(0.0f, 1.0f, 0.5f), 0.5f);
}

CPlayScene::~CPlayScene()
{
} 

void CPlayScene::Render()
{
	glUseProgram(program);

	// Enabling Culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	restartLabel->Render(); // Call before scissor test

	// Enables and declares scissor rectangle
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 50, 1200, 500);

	if (isWireFrame)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	
	//actorCubeObj->Render();
	gameSkybox->Render();
	clothSim->Render();
	//partTest->Render();

	glPolygonMode(GL_FRONT, GL_FILL);

	glBindVertexArray(0);		// Unbinding VAO
	glDisable(GL_SCISSOR_TEST);	// Disables scissor rectangle
	glUseProgram(0);
}

void CPlayScene::Update(GLfloat* deltaTime, ESceneManager* _currentScene)
{
	currentScene = _currentScene;
	gameCamera->Update(*deltaTime);

	if (gameInput->getKeyState('d') || gameInput->getKeyState('D') || gameInput->getClick(0))
	{
		gameCamera->MoveCamera(*deltaTime, 1);
	}
	if (gameInput->getKeyState('a') || gameInput->getKeyState('A') || gameInput->getClick(1))
	{
		gameCamera->MoveCamera(*deltaTime, 0);
	}
	if (gameInput->getKeyState('w') || gameInput->getKeyState('W'))
	{
		gameCamera->CameraRadius(0.01);
	}
	if (gameInput->getKeyState('s') || gameInput->getKeyState('S'))
	{
		gameCamera->CameraRadius(-0.01);
	}

	
	// Wire Frame Check
	if (gameInput->getKeyState('q') || gameInput->getKeyState('Q'))
	{
		isWireFrame = true;
	}
	if (gameInput->getKeyState('e') || gameInput->getKeyState('E'))
	{
		isWireFrame = false;
	}
	
	gameSkybox->Update();
	//actorCubeObj->Update();
	clothSim->Update(*deltaTime);
	//partTest->Update(*deltaTime);

	// Resets every thing in game scene
	if (gameInput->getKeyState('r') || gameInput->getKeyState('R'))
	{
		gameCamera->ResetCamPos(); // This is the only thing in the scene that changes
	}
}

void CPlayScene::TextureGen(const char* textureLocation, GLuint* texture)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	int width, height;
	unsigned char* image1 = SOIL_load_image(textureLocation, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}