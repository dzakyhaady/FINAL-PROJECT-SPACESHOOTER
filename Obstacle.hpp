#pragma once

#include <SFML/Graphics.hpp>

class Obstacle
{
public:
    Obstacle(const sf::Texture& texture, const sf::Image& maskImage, float y, float speed);

    void update();
    void draw(sf::RenderWindow& window) const;

    bool isActive() const;
    void deactivate();
    bool isOffscreen() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getCenter() const;
    bool containsOpaquePixel(sf::Vector2f worldPoint) const;

private:
    sf::Sprite sprite;
    const sf::Image* maskImage = nullptr;
    float speed = 0.f;
    bool active = true;
};
