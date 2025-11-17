#pragma once
#include "AGameObject.h"
#include <vector>

class BGObject : public AGameObject
{
public:
    BGObject(String name);
    void initialize();
    void processInput(sf::Event event);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow* targetWindow) override;

private:
    const float SPEED_MULTIPLIER = 500.0f;
    const float BACKGROUND_SWITCH_TIME = 5.0f; // Show each background for 5 seconds
    const float FADE_DURATION = 1.5f; // Fade takes 1.5 seconds

    std::vector<sf::Texture*> backgroundTextures;
    sf::Sprite* nextSprite = nullptr; // Sprite for the next background during transition

    int currentBackgroundIndex = 0;
    int nextBackgroundIndex = 1;

    float timeSinceLastSwitch = 0.0f;
    float fadeProgress = 0.0f;
    bool isFading = false;

    void startFade();
    void updateFade(float deltaTime);
};