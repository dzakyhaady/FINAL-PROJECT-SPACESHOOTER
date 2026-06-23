#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    Bullet(sf::Vector2f position, sf::Color color);

    void update();
    void draw(sf::RenderWindow& window) const;

    bool isActive() const;
    void deactivate();
    bool canPierce() const;
    void registerHit();
    bool isOffscreen() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getCenter() const;
    bool overlapsOpaquePixel(const class Obstacle& obstacle) const;

private:
    enum class Style
    {
        Basic,
        Blue,
        Red,
        Purple
    };

    Style style = Style::Basic;
    sf::Vector2f position;
    sf::Color color;
    float speed = 0.f;
    int pierceRemaining = 0;
    bool active = true;
};
