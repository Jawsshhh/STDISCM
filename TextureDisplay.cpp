#include "TextureDisplay.h"
#include <iostream>
#include "TextureManager.h"
#include "BaseRunner.h"
#include "GameObjectManager.h"
#include "IconObject.h"

TextureDisplay::TextureDisplay() : AGameObject("TextureDisplay")
{

}

void TextureDisplay::initialize()
{
	threadPool.StartScheduling();
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

		if (numReadyToSpawn > 0) {
			const int SPAWN_BATCH_SIZE = 10;
			int numToSpawnThisBatch = std::min(numReadyToSpawn, SPAWN_BATCH_SIZE);

			std::cout << "[MainThread] === Spawning BATCH of " << numToSpawnThisBatch
				<< " icons (out of " << numReadyToSpawn << " ready) ===" << std::endl;

			for (int i = 0; i < numToSpawnThisBatch; i++) {
				this->spawnObject();
			}
		}

		const int LOAD_BATCH_SIZE = 10;
		int totalTextures = 480; 
		int currentLoadedCount = TextureManager::getInstance()->getNumLoadedStreamTextures();

		if (currentLoadedCount < totalTextures) {
			int texturesToLoad = std::min(LOAD_BATCH_SIZE, totalTextures - currentLoadedCount);

			std::cout << "[MainThread] === Scheduling BATCH of " << texturesToLoad
				<< " texture loads ===" << std::endl;

			for (int i = 0; i < texturesToLoad; i++) {
				int textureIndex = currentLoadedCount + i;
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
	String objectName = "Icon_" + to_string(this->iconList.size());
	IconObject* iconObj = new IconObject(objectName, this->iconList.size());
	this->iconList.push_back(iconObj);

	// Set position
	int IMG_WIDTH = 68;
	int IMG_HEIGHT = 68;
	float x = this->columnGrid * IMG_WIDTH;
	float y = this->rowGrid * IMG_HEIGHT;

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