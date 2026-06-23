#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(const sf::Texture& texture, const sf::Image& maskImage);

    void reset();
    void update();
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getBulletStartPosition() const;
    sf::FloatRect getBounds() const;
    bool containsOpaquePixel(sf::Vector2f worldPoint) const;

private:
    void keepInsideWindow();

    sf::Sprite sprite;
    const sf::Image* maskImage = nullptr;
};
