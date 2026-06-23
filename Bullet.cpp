#include "Bullet.hpp"
#include "Constants.hpp"
#include "Obstacle.hpp"

#include <array>

namespace
{
    constexpr float BasicRadius = 4.f;
    constexpr sf::Vector2f BlueSize{18.f, 12.f};
    constexpr sf::Vector2f RedHitboxSize{28.f, 36.f};
    constexpr sf::Vector2f PurpleSize{46.f, 10.f};

    bool isSameColor(sf::Color color, sf::Color target)
    {
        return color.r == target.r &&
               color.g == target.g &&
               color.b == target.b;
    }
}

Bullet::Bullet(sf::Vector2f startPosition, sf::Color bulletColor)
    : position(startPosition),
      color(bulletColor),
      speed(Constants::BulletSpeed)
{
    if(isSameColor(color, sf::Color(80, 170, 255)))
    {
        style = Style::Blue;
        speed = Constants::BulletSpeed * 1.35f;
    }
    else if(isSameColor(color, sf::Color(255, 80, 80)))
    {
        style = Style::Red;
    }
    else if(isSameColor(color, sf::Color(190, 90, 255)))
    {
        style = Style::Purple;
        pierceRemaining = 1;
    }
}

void Bullet::update()
{
    if(active)
        position.x += speed;
}

void Bullet::draw(sf::RenderWindow& window) const
{
    if(style == Style::Blue)
    {
        sf::ConvexShape triangle(3);
        triangle.setPoint(0, {position.x + BlueSize.x, position.y + BlueSize.y / 2.f});
        triangle.setPoint(1, {position.x, position.y});
        triangle.setPoint(2, {position.x, position.y + BlueSize.y});
        triangle.setFillColor(color);
        triangle.setOutlineColor(sf::Color(190, 230, 255));
        triangle.setOutlineThickness(1.f);
        window.draw(triangle);
        return;
    }

    if(style == Style::Red)
    {
        sf::RectangleShape top({15.f, 5.f});
        sf::RectangleShape middle({13.f, 5.f});
        sf::RectangleShape bottom({15.f, 5.f});
        sf::RectangleShape upperCurve({6.f, 17.f});
        sf::RectangleShape lowerCurve({6.f, 17.f});

        top.setPosition({position.x + 5.f, position.y + 3.f});
        middle.setPosition({position.x + 10.f, position.y + 15.5f});
        bottom.setPosition({position.x + 5.f, position.y + 28.f});
        upperCurve.setPosition({position.x + 17.f, position.y + 3.f});
        lowerCurve.setPosition({position.x + 17.f, position.y + 15.5f});

        for(auto* part : {&top, &middle, &bottom, &upperCurve, &lowerCurve})
        {
            part->setFillColor(color);
            part->setOutlineColor(sf::Color(255, 185, 185));
            part->setOutlineThickness(1.f);
            window.draw(*part);
        }
        return;
    }

    if(style == Style::Purple)
    {
        sf::RectangleShape glow(PurpleSize);
        glow.setPosition(position);
        glow.setFillColor(sf::Color(150, 60, 255, 170));

        sf::RectangleShape core({PurpleSize.x, 4.f});
        core.setPosition({position.x, position.y + 3.f});
        core.setFillColor(sf::Color(245, 225, 255));

        sf::CircleShape front(PurpleSize.y / 2.f);
        sf::CircleShape back(PurpleSize.y / 2.f);
        front.setPosition({position.x + PurpleSize.x - PurpleSize.y / 2.f, position.y});
        back.setPosition({position.x - PurpleSize.y / 2.f, position.y});
        front.setFillColor(sf::Color(210, 135, 255));
        back.setFillColor(sf::Color(120, 45, 210, 180));

        window.draw(back);
        window.draw(glow);
        window.draw(core);
        window.draw(front);
        return;
    }

    sf::CircleShape shape(BasicRadius);
    shape.setFillColor(color);
    shape.setPosition(position);
    window.draw(shape);
}

bool Bullet::isActive() const
{
    return active;
}

void Bullet::deactivate()
{
    active = false;
}

bool Bullet::canPierce() const
{
    return pierceRemaining > 0;
}

void Bullet::registerHit()
{
    if(canPierce())
        pierceRemaining--;
    else
        deactivate();
}

bool Bullet::isOffscreen() const
{
    return position.x > Constants::WindowWidth + 80.f;
}

sf::FloatRect Bullet::getBounds() const
{
    if(style == Style::Blue)
        return {position, BlueSize};

    if(style == Style::Red)
        return {position, RedHitboxSize};

    if(style == Style::Purple)
        return {{position.x - PurpleSize.y / 2.f, position.y}, {PurpleSize.x + PurpleSize.y, PurpleSize.y}};

    return {position, {BasicRadius * 2.f, BasicRadius * 2.f}};
}

sf::Vector2f Bullet::getCenter() const
{
    auto bounds = getBounds();

    return {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
}

bool Bullet::overlapsOpaquePixel(const Obstacle& obstacle) const
{
    auto bounds = getBounds();
    const std::array<sf::Vector2f, 9> samplePoints{{
        getCenter(),
        {bounds.position.x, bounds.position.y},
        {bounds.position.x + bounds.size.x, bounds.position.y},
        {bounds.position.x, bounds.position.y + bounds.size.y},
        {bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y},
        {bounds.position.x + bounds.size.x / 2.f, bounds.position.y},
        {bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y},
        {bounds.position.x, bounds.position.y + bounds.size.y / 2.f},
        {bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y / 2.f}
    }};

    for(const auto& point : samplePoints)
    {
        if(obstacle.containsOpaquePixel(point))
            return true;
    }

    return false;
}
