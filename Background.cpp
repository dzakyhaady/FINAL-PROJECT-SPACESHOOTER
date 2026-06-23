#include "Background.hpp"
#include "Constants.hpp"

Background::Background(const sf::Texture& texture)
    : firstSprite(texture),
      secondSprite(texture)
{
    firstSprite.setScale({
        Constants::WindowWidth / texture.getSize().x,
        Constants::WindowHeight / texture.getSize().y
    });

    secondSprite.setScale(firstSprite.getScale());

    width = firstSprite.getGlobalBounds().size.x;
    firstSprite.setPosition({0.f, 0.f});
    secondSprite.setPosition({width, 0.f});
}

void Background::update()
{
    firstSprite.move({-Constants::BackgroundSpeed, 0.f});
    secondSprite.move({-Constants::BackgroundSpeed, 0.f});

    if(firstSprite.getPosition().x + width <= 0.f)
        firstSprite.setPosition({secondSprite.getPosition().x + width, 0.f});

    if(secondSprite.getPosition().x + width <= 0.f)
        secondSprite.setPosition({firstSprite.getPosition().x + width, 0.f});
}

void Background::draw(sf::RenderWindow& window) const
{
    window.draw(firstSprite);
    window.draw(secondSprite);
}
