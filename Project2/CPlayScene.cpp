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

	const char* cubeFileLocation = "Resources/Textures/dropSprite.png";
	TextureGen(cubeFileLocation, &dropTex);

	const char* waterFileLocation = "Resources/Textures/resetSprite.png";
	TextureGen(waterFileLocation, &resetTex);

	const char* increaseLocation = "Resources/Textures/increase.png";
	TextureGen(increaseLocation, &increasePinTex);

	const char* decreaseLocation = "Resources/Textures/decrease.png";
	TextureGen(decreaseLocation, &decreasePinTex);

	const char* applyLocation = "Resources/Textures/applySprite.png";
	TextureGen(applyLocation, &applyWindTex);

	const char* windRightLocation = "Resources/Textures/windRight.png";
	TextureGen(windRightLocation, &windleftTex);

	const char* windLeftLocation = "Resources/Textures/windLeft.png";
	TextureGen(windLeftLocation, &windrightTex);

	// Creates Mesh
	actorPyramid = new CPyramid();
	actorCube = new CCube(0.1f);

	// Create Game Actors
	actorCubeObj = new CObject(&programNoFog, actorCube->GetVAO(), actorCube->GetIndiceCount(), gameCamera, &actorTex);
	
	// Cloth Sim
	particleHeight = 10.0f;
	clothSim = new CCloth(particleHeight, 10.0f, 10.0f, 10.0f, 200.0f, 1.0f, glm::vec3(0.0f, 5.0f, 0.0f), gameCamera, gameInput);

	// Create Skybox
	gameSkybox = new CSkybox(&programSkybox, gameCamera);

	// Labels
	//restartLabel = new CTextLabel("Press 'R' to restart", "Resources/Fonts/arial.ttf", glm::vec2(10.0f, 570.0f), glm::vec3(0.0f, 1.0f, 0.5f), 0.5f);
	InitButtons();
}

CPlayScene::~CPlayScene()
{
} 

void CPlayScene::Render()
{
	glUseProgram(program);

	// Buttons
	dropObj->Render2D();
	resetObj->Render2D();
	increasePinObj->Render2D();
	decreasePinObj->Render2D();
	windLeftObj->Render2D();
	windRightObj->Render2D();
	applyWindObj->Render2D();

	// Enabling Culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//restartLabel->Render(); // Call before scissor test

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

	ButtonChecks();

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
		//gameCamera->ResetCamPos(); // This is the only thing in the scene that changes
		delete clothSim;
		clothSim = new CCloth(10.0f, 10.0f, 10.0f, 10.0f, 200.0f, 1.0f, glm::vec3(0.0f, 5.0f, 0.0f), gameCamera, gameInput);

	}
}

bool CPlayScene::Button(float width, float height, CObject* _buttonObj)
{
	mouseX = gameInput->getMouseX();
	mouseY = gameInput->getMouseY();

	float offSetX = Utils::SCR_WIDTH / 2;
	float offSetY = Utils::SCR_HEIGHT / 2;

	if (-(offSetX - mouseX) > _buttonObj->objPosition.x - width / 2 &&
		-(offSetX - mouseX) < _buttonObj->objPosition.x + width / 2 &&
		offSetY - mouseY > _buttonObj->objPosition.y - height / 2 &&
		offSetY - mouseY < _buttonObj->objPosition.y + height / 2 &&
		(gameInput->GetFirstDown(0) || gameInput->GetFirstDown(1)))
	{
		return(true);
	}
	else
	{
		return(false);
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

void CPlayScene::ButtonChecks()
{
	// Mouse has clicked button check
	dropObj->Update();
	if (Button(150, 50, dropObj))
	{
		clothSim->allPartsInCloth[0]->isFrozen = false;
		clothSim->allPartsInCloth[clothSim->allPartsInCloth.size() - clothSim->particleWidth]->isFrozen = false;
	}

	resetObj->Update();
	if (Button(150, 50, resetObj))
	{
		delete clothSim;
		clothSim = new CCloth(10.0f, 10.0f, 10.0f, 10.0f, 200.0f, 1.0f, glm::vec3(0.0f, 5.0f, 0.0f), gameCamera, gameInput);
	}

	increasePinObj->Update();
	if (Button(50, 50, increasePinObj))
	{
		particleHeight++;
		delete clothSim;
		clothSim = new CCloth(10.0f, 10.0f, 10.0f, particleHeight, 200.0f, 1.0f, glm::vec3(0.0f, 5.0f, 0.0f), gameCamera, gameInput);
	}

	decreasePinObj->Update();
	if (Button(50, 50, decreasePinObj) && particleHeight > 1)
	{
		particleHeight--;
		delete clothSim;
		clothSim = new CCloth(10.0f, 10.0f, 10.0f, particleHeight, 200.0f, 1.0f, glm::vec3(0.0f, 5.0f, 0.0f), gameCamera, gameInput);
	}

	windRightObj->Update();
	if (Button(50, 50, windRightObj))
	{
		clothSim->wind = 1;
	}

	windLeftObj->Update();
	if (Button(50, 50, windLeftObj))
	{
		clothSim->wind = 2;
	}

	applyWindObj->Update();
	if (Button(150, 50, applyWindObj))
	{
		clothSim->apply = !clothSim->apply;
	}
}

void CPlayScene::InitButtons()
{
	// Drop Cloth
	dropPlane = new CPlane(150.0f, 50.0f);
	dropObj = new CObject(&program, dropPlane->GetVAO(), dropPlane->GetIndiceCount(), gameCamera, &dropTex);
	dropObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 120;
	dropObj->objPosition.y -= 250;

	// Reset Cloth
	resetObj = new CObject(&program, dropPlane->GetVAO(), dropPlane->GetIndiceCount(), gameCamera, &resetTex);
	resetObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 320;
	resetObj->objPosition.y -= 250;

	// Incerase Pins
	increasePlane = new CPlane(50.0f, 50.0f);
	increasePinObj = new CObject(&program, increasePlane->GetVAO(), increasePlane->GetIndiceCount(), gameCamera, &increasePinTex);
	increasePinObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 170;
	increasePinObj->objPosition.y -= 150;

	// Decrease Pins
	decreasePinObj = new CObject(&program, increasePlane->GetVAO(), increasePlane->GetIndiceCount(), gameCamera, &decreasePinTex);
	decreasePinObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 70;
	decreasePinObj->objPosition.y -= 150;

	// Wind Right
	windRightObj = new CObject(&program, increasePlane->GetVAO(), increasePlane->GetIndiceCount(), gameCamera, &windrightTex);
	windRightObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 580;
	windRightObj->objPosition.y -= 150;

	// Wind Left
	windLeftObj = new CObject(&program, increasePlane->GetVAO(), increasePlane->GetIndiceCount(), gameCamera, &windleftTex);
	windLeftObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 470;
	windLeftObj->objPosition.y -= 150;

	// Appy Wind
	applyWindObj = new CObject(&program, dropPlane->GetVAO(), dropPlane->GetIndiceCount(), gameCamera, &applyWindTex);
	applyWindObj->objPosition.x -= (Utils::SCR_WIDTH / 2) - 520;
	applyWindObj->objPosition.y -= 250;
}