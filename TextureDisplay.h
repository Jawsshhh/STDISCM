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

	ThreadPool threadPool = ThreadPool(30);
	AnimatedCharacter* loadingCharacter = nullptr;
	LoadingText* loadingText = nullptr;

	const int TOTAL_TEXTURES = 300;
	const float STREAMING_LOAD_DELAY = 500.0f;
	float ticks = 0.0f;

	int columnGrid = 0;
	int rowGrid = 0;

	const int MAX_COLUMN = 28;
	const int MAX_ROW = 22;

	std::mutex guard;
	bool allIconsVisible = false;  // ADD THIS
	bool loadingComplete = false;  // ADD THIS
	bool bgTransitionStarted = false;  // ADD THIS
	float delayTimer = 0.0f;  // ADD THIS
	const float ICON_FADE_DELAY = 2.0f;  // ADD THIS - delay in seconds after bg transition
	const float ICON_FADE_DURATION = 1.5f;  // ADD THIS - how long icons take to fade in
	float iconFadeProgress = 0.0f;  // ADD THIS

	void spawnObject(); // Restore spawn method
	void updateLoadingProgress();
	void updateIconFadeIn(sf::Time deltaTime);  // ADD THIS

};