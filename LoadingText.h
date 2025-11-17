#pragma once
#include "AGameObject.h"
#include <vector>

class LoadingText : public AGameObject
{
public:
    LoadingText(String name);
    void initialize() override;
    void processInput(sf::Event event) override;
    void update(sf::Time deltaTime) override;

private:
    std::vector<sf::Texture*> animationFrames;
    int currentFrame = 0;
    float animationSpeed = 0.15f; // Time between frames (adjust for speed)
    float timeSinceLastFrame = 0.0f;

    float xPosition = 100.0f;  // X position
    float yPosition = 100.0f;  // Y position

    void loadAnimationFrames();
    void updateAnimation(sf::Time deltaTime);
};