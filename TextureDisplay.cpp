#include "TextureDisplay.h"
#include <iostream>
#include "TextureManager.h"
#include "BaseRunner.h"
#include "GameObjectManager.h"
#include "IconObject.h"
TextureDisplay::TextureDisplay(): AGameObject("TextureDisplay")
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
	this->ticks += BaseRunner::TIME_PER_FRAME.asMilliseconds();
	
	ticks += deltaTime.asMilliseconds();

	if (ticks > STREAMING_LOAD_DELAY) {
		int texCount = TextureManager::getInstance()->getNumLoadedStreamTextures();
		if (texCount < 200) {
			LoadAssetThread* asset = new LoadAssetThread(texCount, this);
			//asset->start();
			threadPool.ScheduleTask(asset);
		}
		ticks = 0;
	}
	//<code here for spawning icon object periodically>
}

void TextureDisplay::OnFinishedExecution()
{
	this->spawnObject();
}

void TextureDisplay::spawnObject()
{
	guard.lock();
	String objectName = "Icon_" + to_string(this->iconList.size());
	IconObject* iconObj = new IconObject(objectName, this->iconList.size());
	this->iconList.push_back(iconObj);

	//set position
	int IMG_WIDTH = 68; int IMG_HEIGHT = 68;
	float x = this->columnGrid * IMG_WIDTH;
	float y = this->rowGrid * IMG_HEIGHT;

	std::cout << "Set position: " << x << " " << y << std::endl;

	this->columnGrid++;
	if(this->columnGrid == this->MAX_COLUMN)
	{
		this->columnGrid = 0;
		this->rowGrid++;
	}
	GameObjectManager::getInstance()->addObject(iconObj);
	iconObj->setPosition(x, y);
	guard.unlock();

}
