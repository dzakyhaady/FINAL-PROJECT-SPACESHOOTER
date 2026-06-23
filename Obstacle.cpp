#include "Obstacle.hpp"
#include "Constants.hpp"

Obstacle::Obstacle(const sf::Texture& texture, const sf::Image& maskImage, float y, float speed)
    : sprite(texture),
      maskImage(&maskImage),
      speed(speed)
{
    sprite.setScale({0.17f, 0.17f});
    sprite.setColor(sf::Color(165, 190, 170));
    sprite.setPosition({Constants::WindowWidth + 100.f, y});
}

void Obstacle::update()
{
    if(active)
        sprite.move({-speed, 0.f});
}

void Obstacle::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

bool Obstacle::isActive() const
{
    return active;
}

void Obstacle::deactivate()
{
    active = false;
}

bool Obstacle::isOffscreen() const
{
    return sprite.getPosition().x < -150.f;
}

sf::FloatRect Obstacle::getBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Obstacle::getPosition() const
{
    return sprite.getPosition();
}

sf::Vector2f Obstacle::getCenter() const
{
    auto bounds = sprite.getGlobalBounds();

    return {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
}

bool Obstacle::containsOpaquePixel(sf::Vector2f worldPoint) const
{
    if(maskImage == nullptr)
        return false;

    auto localPoint = sprite.getInverseTransform().transformPoint(worldPoint);
    auto imageSize = maskImage->getSize();

    if(localPoint.x < 0.f ||
       localPoint.y < 0.f ||
       localPoint.x >= imageSize.x ||
       localPoint.y >= imageSize.y)
    {
        return false;
    }

    auto pixel = maskImage->getPixel({
        static_cast<unsigned>(localPoint.x),
        static_cast<unsigned>(localPoint.y)
    });

    return pixel.a > 30;
}
