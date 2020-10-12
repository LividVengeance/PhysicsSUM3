#include "CCloth.h"

CCloth::CCloth(float _clothWidth, float _clothHeight, int _particleWidth,
	int _particleHeight, float _mass, float _damping, glm::vec3 _clothPos,
	CCamera* _gameCamera, CInput* _gameInputs)
{
	clothHeight = _clothHeight;
	clothWidth = _clothWidth;
	particleHeight = _particleHeight;
	particleWidth = _particleWidth;
	mass = _mass;
	damping = _damping;
	clothPos = _clothPos;
	gameCamera = _gameCamera;
	gameInput = _gameInputs;

	program = CShaderLoader::CreateProgram("Resources/Shaders/Basic.vs",
		"Resources/Shaders/Basic.fs");

	// Gen Textures For Actor
	const char* fileLocationBullet = "Resources/Textures/BackgroundSprite.png";
	TextureGen(fileLocationBullet, &texture);

	partMesh = new CCube(0.10f);

	// Particle Var Setup
	float distPartX = clothWidth / particleWidth;
	float distPartY = clothHeight / particleHeight;
	float numPart = particleWidth * particleHeight;
	float partMass = numPart / mass;
	float partDamping = 1 - damping;

	// Creates all Particles in Cloth
	for (int i = 0; i < particleHeight; i++)
	{
		for (int j = 0; j < particleWidth; j++)
		{
			//glm::vec3 partPos = (glm::vec3)(modelMatrix * glm::vec4(i * distPartX, -j * distPartY, 0, 1));
			glm::vec3 partPos = vec3(clothPos + vec3(i * distPartX, -j * distPartY, 0.0f));
			allPartsInCloth.push_back(new CParticle(partPos, partMass, partDamping, partMesh, &program, &texture, gameCamera));
		}
	}

	// Creates the Constraints Between all Particles in Cloth
	for (int i = 0; i < numPart; i++)
	{
		// Vertical
		if (!(i < particleWidth))
		{
			allConsnInCloth.push_back(new CConstraints(allPartsInCloth[i], allPartsInCloth[i - particleWidth]));
		}
		if (!(i < particleWidth * 2))
		{
			allConsnInCloth.push_back(new CConstraints(allPartsInCloth[i], allPartsInCloth[i - (particleWidth * 2)]));
		}

		// Horizontal
		if (!(i % particleWidth == 0))
		{
			allConsnInCloth.push_back(new CConstraints(allPartsInCloth[i], allPartsInCloth[i - 1]));
		}
		if (!((i % particleWidth == 0) || (i - 1) % particleWidth == 0))
		{
			allConsnInCloth.push_back(new CConstraints(allPartsInCloth[i], allPartsInCloth[i - 1]));
		}

		// Diagonal
		if (!(i < particleWidth) && !(i % particleWidth == 0))
		{
			allConsnInCloth.push_back(new CConstraints(allPartsInCloth[i], allPartsInCloth[i - 1 - particleWidth]));
		}
		if (!(i < particleWidth) && !((i + 1) % particleWidth == 0))
		{
			allConsnInCloth.push_back(new CConstraints(allPartsInCloth[i], allPartsInCloth[i + 1 - particleWidth]));
		}
	}

	allPartsInCloth[numPart / 2 + particleWidth / 2]->force.z -= 1;

	// Pins the Top Left and Right - To Keep Cloth Up
	allPartsInCloth[0]->isFrozen = true;
	allPartsInCloth[allPartsInCloth.size() - particleWidth]->isFrozen = true;
	currentSelected = 1;

	floor = new CFloor(glm::vec3(0.0f, -4.0f, 0.0f), 100.0f, 100.0f);
}

CCloth::~CCloth()
{
	for (std::vector<CParticle>::size_type i = 0; i < allPartsInCloth.size(); i++)
	{
		delete allPartsInCloth[i];
	}
}

void CCloth::Update(float _deltaTime)
{
	// Applying Gravity to All Particles
	for (std::vector<CParticle>::size_type i = 0; i < allPartsInCloth.size(); i++)
	{
		allPartsInCloth[i]->force += glm::vec3(0, -0.3, 0) * _deltaTime;

		// Adding wind
		if ((gameInput->getKeyState('f') || gameInput->getKeyState('F')))
		{
			allPartsInCloth[i]->force += glm::vec3(0, 0, -3) * _deltaTime;
		}
		if ((gameInput->getKeyState('g') || gameInput->getKeyState('G')))
		{
			allPartsInCloth[i]->force += glm::vec3(0, 0, 3) * _deltaTime;
		}
	}
	
	for (int i = 0; i < 20; i++)
	{
		for (std::vector<CConstraints>::size_type j = 0; j < allConsnInCloth.size(); j++)
		{
			allConsnInCloth[j]->Satisfy();
		}
	}
	
	// Apply Forces to All Particles
	for (std::vector<CParticle>::size_type i = 0; i < allPartsInCloth.size(); i++)
	{
		allPartsInCloth[i]->Update(_deltaTime);
	}

	floor->Update(allPartsInCloth);
	PinUpdates();

	// Reset the cloth
	if (gameInput->getKeyState('r') || gameInput->getKeyState('R'))
	{
		//ResetCloth();
	}
}

void CCloth::Render()
{
	// Render all the particles on the cloth
	for (std::vector<CParticle>::size_type i = 0; i < allPartsInCloth.size(); i++)
	{
		//if (i == 1)
		{
			allPartsInCloth[i]->Render();
		}
			
	}

	//for (std::vector<CConstraints>::size_type i = 0; i < allConsnInCloth.size(); i++)
	//{
	//	allConsnInCloth[i].Render();
	//}
}

void CCloth::PinUpdates()
{
	// Updates the current selected pin
	if (gameInput->getKeyState('1'))
	{
		currentSelected = 1;
	}
	if (gameInput->getKeyState('2'))
	{
		currentSelected = 2;
	}

	// Relesae all pins
	if (gameInput->getKeyState('u') || gameInput->getKeyState('U'))
	{
		allPartsInCloth[0]->isFrozen = false;
		allPartsInCloth[allPartsInCloth.size() - particleWidth]->isFrozen = false;
	}

	// Release the pin of sellected pin
	if ((gameInput->getKeyState('t') || gameInput->getKeyState('T')) && (currentSelected == 1))
	{
		allPartsInCloth[0]->isFrozen = !allPartsInCloth[0]->isFrozen;
	}
	if ((gameInput->getKeyState('t') || gameInput->getKeyState('T')) && (currentSelected == 2))
	{
		allPartsInCloth[allPartsInCloth.size() - particleWidth]->isFrozen = !allPartsInCloth[allPartsInCloth.size() - particleWidth]->isFrozen;
	}

	

	// Moves the selected pin
	if ((gameInput->getKeyState('o') || gameInput->getKeyState('O')) && (currentSelected == 1))
	{
		allPartsInCloth[0]->pos.x -= 0.1f;
	}
	if ((gameInput->getKeyState('p') || gameInput->getKeyState('P')) && (currentSelected == 1))
	{
		allPartsInCloth[0]->pos.x += 0.1f;
	}
	if ((gameInput->getKeyState('o') || gameInput->getKeyState('O')) && (currentSelected == 2))
	{
		allPartsInCloth[allPartsInCloth.size() - particleWidth]->pos.x -= 0.1f;
	}
	if ((gameInput->getKeyState('p') || gameInput->getKeyState('P')) && (currentSelected == 2))
	{
		allPartsInCloth[allPartsInCloth.size() - particleWidth]->pos.x += 0.1f;
	}
}

void CCloth::ResetCloth()
{
	for (std::vector<CParticle>::size_type i = 0; i < allPartsInCloth.size(); i++)
	{
		delete &allPartsInCloth[i];
	}
}

void CCloth::TextureGen(const char* textureLocation, GLuint* texture)
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