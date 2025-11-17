#include "TextureDisplay.h"
#include <iostream>
#include "TextureManager.h"
#include "BaseRunner.h"
#include "GameObjectManager.h"
#include "IconObject.h"

TextureDisplay::TextureDisplay() : AGameObject("TextureDisplay")
{
}

TextureDisplay::~TextureDisplay()
{
	// Clean up icons
	for (auto icon : iconList)
	{
		delete icon;
	}

	if (loadingCharacter != nullptr)
	{
		delete loadingCharacter;
	}
	if (loadingText != nullptr)
	{
		delete loadingText;
	}
}

void TextureDisplay::initialize()
{
	threadPool.StartScheduling();

	// Create animated character
	loadingCharacter = new AnimatedCharacter("LoadingCharacter");
	GameObjectManager::getInstance()->addObject(loadingCharacter);

	std::cout << "Loading character created" << std::endl;

	// Create loading text
	loadingText = new LoadingText("LoadingText");
	GameObjectManager::getInstance()->addObject(loadingText);

	std::cout << "Loading text created" << std::endl;
}

void TextureDisplay::processInput(sf::Event event)
{
}

void TextureDisplay::update(sf::Time deltaTime)
{
	this->ticks += deltaTime.asMilliseconds();

	if (this->ticks > this->STREAMING_LOAD_DELAY) {
		this->ticks = 0.0f;

		int spawnedIconCount = this->iconList.size();
		int loadedTextureCount = TextureManager::getInstance()->getNumLoadedStreamTextures();
		int numReadyToSpawn = loadedTextureCount - spawnedIconCount;

		// Spawn icons in batches
		if (numReadyToSpawn > 0) {
			const int SPAWN_BATCH_SIZE = 10;
			int numToSpawnThisBatch = std::min(numReadyToSpawn, SPAWN_BATCH_SIZE);

			std::cout << "[MainThread] === Spawning BATCH of " << numToSpawnThisBatch
				<< " icons (out of " << numReadyToSpawn << " ready) ===" << std::endl;

			for (int i = 0; i < numToSpawnThisBatch; i++) {
				this->spawnObject();
			}
		}

		// Update loading progress for character animation
		updateLoadingProgress();

		// Load textures in batches
		const int LOAD_BATCH_SIZE = 10;
		int totalTextures = 480;

		if (loadedTextureCount < totalTextures)
		{
			int texturesToLoad = std::min(LOAD_BATCH_SIZE, totalTextures - loadedTextureCount);

			std::cout << "[MainThread] === Scheduling BATCH of " << texturesToLoad
				<< " texture loads ===" << std::endl;

			for (int i = 0; i < texturesToLoad; i++)
			{
				int textureIndex = loadedTextureCount + i;
				LoadAssetThread* asset = new LoadAssetThread(textureIndex, this);
				threadPool.ScheduleTask(asset);
			}
		}
	}
}

void TextureDisplay::OnFinishedExecution()
{
	std::cout << "[LoadThread] Texture loaded. Total: "
		<< TextureManager::getInstance()->getNumLoadedStreamTextures() << std::endl;
}

void TextureDisplay::spawnObject()
{
	guard.lock();

	String objectName = "Icon_" + std::to_string(this->iconList.size());
	IconObject* iconObj = new IconObject(objectName, this->iconList.size());
	this->iconList.push_back(iconObj);

	// Set position in grid with offset
	int IMG_WIDTH = 68;
	int IMG_HEIGHT = 68;
	int OFFSET_X = 0;    // Horizontal offset from left edge
	int OFFSET_Y = -60;   // Vertical offset from top (adjust this to move down)

	float x = OFFSET_X + (this->columnGrid * IMG_WIDTH);
	float y = OFFSET_Y + (this->rowGrid * IMG_HEIGHT);

	std::cout << "Set position: " << x << " " << y << std::endl;

	this->columnGrid++;
	if (this->columnGrid == this->MAX_COLUMN)
	{
		this->columnGrid = 0;
		this->rowGrid++;
	}

	GameObjectManager::getInstance()->addObject(iconObj);
	iconObj->setPosition(x, y);

	guard.unlock();
}

void TextureDisplay::updateLoadingProgress()
{
	if (loadingCharacter == nullptr) return;

	int loadedCount = TextureManager::getInstance()->getNumLoadedStreamTextures();
	float progress = static_cast<float>(loadedCount) / static_cast<float>(TOTAL_TEXTURES);

	loadingCharacter->updateProgress(progress);

	// Optional: Remove character when loading is complete
	if (loadedCount >= TOTAL_TEXTURES)
	{
		std::cout << "Loading complete!" << std::endl;
		// You could hide or remove the character here
	}
}