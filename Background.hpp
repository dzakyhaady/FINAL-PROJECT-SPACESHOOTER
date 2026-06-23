#pragma once

#include <SFML/Graphics.hpp>

class Background
{
public:
    explicit Background(const sf::Texture& texture);

    void update();
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite firstSprite;
    sf::Sprite secondSprite;
    float width = 0.f;
};
