#include "BGObject.h"
#include <iostream>
#include "TextureManager.h"
#include "BaseRunner.h"

BGObject::BGObject(string name) : AGameObject(name)
{
}

void BGObject::initialize()
{
    std::cout << "Declared as " << this->getName() << "\n";

    // Load multiple background textures
    sf::Texture* bg1 = TextureManager::getInstance()->getFromTextureMap("bg1", 0);
    if (bg1 != nullptr) backgroundTextures.push_back(bg1);

    // Add more backgrounds - make sure these are in your assets.txt!
    sf::Texture* bg2 = TextureManager::getInstance()->getFromTextureMap("bg2", 0);
    if (bg2 != nullptr) backgroundTextures.push_back(bg2);

    sf::Texture* bg3 = TextureManager::getInstance()->getFromTextureMap("bg3", 0);
    if (bg3 != nullptr) backgroundTextures.push_back(bg3);

    // Set up the main sprite
    this->sprite = new sf::Sprite();
    this->nextSprite = new sf::Sprite();

    if (!backgroundTextures.empty())
    {
        sf::Texture* texture = backgroundTextures[0];
        texture->setRepeated(true);
        this->sprite->setTexture(*texture);
        this->sprite->setTextureRect(sf::IntRect(0, 0, BaseRunner::WINDOW_WIDTH, BaseRunner::WINDOW_HEIGHT * 8));
        this->setPosition(0, -BaseRunner::WINDOW_HEIGHT * 7);

        // Setup next sprite if we have multiple backgrounds
        if (backgroundTextures.size() > 1)
        {
            nextBackgroundIndex = 1;
            sf::Texture* nextTexture = backgroundTextures[nextBackgroundIndex];
            nextTexture->setRepeated(true);
            this->nextSprite->setTexture(*nextTexture);
            this->nextSprite->setTextureRect(sf::IntRect(0, 0, BaseRunner::WINDOW_WIDTH, BaseRunner::WINDOW_HEIGHT * 8));
            this->nextSprite->setPosition(0, -BaseRunner::WINDOW_HEIGHT * 7);

            // Start with next sprite invisible
            sf::Color color = this->nextSprite->getColor();
            color.a = 0;
            this->nextSprite->setColor(color);
        }
    }
}

void BGObject::processInput(sf::Event event)
{
}

void BGObject::update(sf::Time deltaTime)
{
    // Handle background switching with fade
    if (backgroundTextures.size() > 1)
    {
        if (!isFading)
        {
            timeSinceLastSwitch += deltaTime.asSeconds();

            if (timeSinceLastSwitch >= BACKGROUND_SWITCH_TIME)
            {
                startFade();
                timeSinceLastSwitch = 0.0f;
            }
        }
        else
        {
            updateFade(deltaTime.asSeconds());
        }
    }

    // Make BG scroll slowly
    //sf::Vector2f position = this->getPosition();
    //position.y += this->SPEED_MULTIPLIER * deltaTime.asSeconds();

    //this->setPosition(position.x, position.y);

    // Update next sprite position too
    //if (this->nextSprite != nullptr)
    //{
    //    this->nextSprite->setPosition(position.x, position.y);
    //}

    //sf::Vector2f localPos = this->sprite->getPosition();
    //if (localPos.y * deltaTime.asSeconds() > 0)
    //{
    //    this->setPosition(0, -BaseRunner::WINDOW_HEIGHT * 7);
    //    if (this->nextSprite != nullptr)
     //   {
     //       this->nextSprite->setPosition(0, -BaseRunner::WINDOW_HEIGHT * 7);
     //   }
   // }
}

void BGObject::draw(sf::RenderWindow* targetWindow)
{
    if (this->sprite != nullptr)
    {
        // Draw current background
        targetWindow->draw(*this->sprite);

        // Draw next background on top if fading
        if (isFading && this->nextSprite != nullptr)
        {
            targetWindow->draw(*this->nextSprite);
        }
    }
}

void BGObject::startFade()
{
    if (backgroundTextures.size() <= 1) return;

    isFading = true;
    fadeProgress = 0.0f;

    std::cout << "Starting fade from " << currentBackgroundIndex
        << " to " << nextBackgroundIndex << std::endl;
}

void BGObject::updateFade(float deltaTime)
{
    fadeProgress += deltaTime / FADE_DURATION;

    if (fadeProgress >= 1.0f)
    {
        // Fade complete - swap backgrounds
        fadeProgress = 1.0f;
        isFading = false;

        // Update current background
        currentBackgroundIndex = nextBackgroundIndex;
        nextBackgroundIndex = (nextBackgroundIndex + 1) % backgroundTextures.size();

        // Swap sprites
        sf::Texture* currentTexture = backgroundTextures[currentBackgroundIndex];
        currentTexture->setRepeated(true);
        this->sprite->setTexture(*currentTexture);

        // Setup next sprite for next transition
        sf::Texture* nextTexture = backgroundTextures[nextBackgroundIndex];
        nextTexture->setRepeated(true);
        this->nextSprite->setTexture(*nextTexture);

        // Reset alpha
        sf::Color color = this->nextSprite->getColor();
        color.a = 0;
        this->nextSprite->setColor(color);

        std::cout << "Fade complete. Current: " << currentBackgroundIndex << std::endl;
    }
    else
    {
        // Update alpha for smooth fade
        sf::Color color = this->nextSprite->getColor();
        color.a = static_cast<sf::Uint8>(fadeProgress * 255);
        this->nextSprite->setColor(color);
    }
}