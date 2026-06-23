#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct ExplosionParticle
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Color startColor;
    sf::Color endColor;
    float lifetime = 1.f;
    float age = 0.f;
    float spin = 0.f;
};

class Explosion
{
public:
    explicit Explosion(sf::Vector2f position);

    void update();
    void draw(sf::RenderWindow& window) const;
    bool isFinished() const;

private:
    std::vector<ExplosionParticle> particles;
};
