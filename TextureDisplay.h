#pragma once
#include "AGameObject.h"
#include "LoadAssetThread.h"

#include <mutex>
#include "ThreadPool.h"
#include "AnimatedCharacter.h"
#include "LoadingText.h"

class IconObject;

class TextureDisplay : public AGameObject, public IExecutionEvent
{
public:
	TextureDisplay();
	~TextureDisplay();
	void initialize();
	void processInput(sf::Event event);
	void update(sf::Time deltaTime);
	void OnFinishedExecution() override;

private:
	typedef std::vector<IconObject*> IconList;
	IconList iconList; // Restore the icon list

	ThreadPool threadPool = ThreadPool(20);
	AnimatedCharacter* loadingCharacter = nullptr;
	LoadingText* loadingText = nullptr;

	const int TOTAL_TEXTURES = 480;
	const float STREAMING_LOAD_DELAY = 750.0f;
	float ticks = 0.0f;

	int columnGrid = 0;
	int rowGrid = 0;

	const int MAX_COLUMN = 28;
	const int MAX_ROW = 22;

	std::mutex guard;

	void spawnObject(); // Restore spawn method
	void updateLoadingProgress();
};