#include "Explosion.hpp"

#include <cmath>
#include <cstdint>
#include <random>

namespace
{
    constexpr float Pi = 3.1415926535f;

    float randomFloat(std::mt19937& rng, float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    sf::Color lerpColor(sf::Color start, sf::Color end, float t)
    {
        auto lerp = [t](std::uint8_t a, std::uint8_t b)
        {
            return static_cast<std::uint8_t>(a + (b - a) * t);
        };

        return {
            lerp(start.r, end.r),
            lerp(start.g, end.g),
            lerp(start.b, end.b),
            lerp(start.a, end.a)
        };
    }
}

Explosion::Explosion(sf::Vector2f position)
{
    static std::mt19937 rng(std::random_device{}());

    particles.reserve(42);

    for(int i = 0; i < 26; ++i)
    {
        const float angle = randomFloat(rng, 0.f, Pi * 2.f);
        const float speed = randomFloat(rng, 2.2f, 7.5f);
        const float radius = randomFloat(rng, 2.f, 5.5f);

        ExplosionParticle particle;
        particle.shape.setRadius(radius);
        particle.shape.setOrigin({radius, radius});
        particle.shape.setPosition(position);
        particle.shape.setFillColor(sf::Color(255, 210, 70));
        particle.velocity = {
            std::cos(angle) * speed,
            std::sin(angle) * speed
        };
        particle.startColor = sf::Color(255, 190, 45, 245);
        particle.endColor = sf::Color(130, 45, 20, 0);
        particle.lifetime = randomFloat(rng, 0.35f, 0.75f);
        particle.spin = randomFloat(rng, -9.f, 9.f);
        particles.push_back(particle);
    }

    for(int i = 0; i < 12; ++i)
    {
        const float angle = randomFloat(rng, 0.f, Pi * 2.f);
        const float speed = randomFloat(rng, 0.8f, 3.2f);
        const float radius = randomFloat(rng, 3.5f, 8.f);

        ExplosionParticle smoke;
        smoke.shape.setRadius(radius);
        smoke.shape.setOrigin({radius, radius});
        smoke.shape.setPosition(position);
        smoke.shape.setFillColor(sf::Color(95, 95, 95, 150));
        smoke.velocity = {
            std::cos(angle) * speed,
            std::sin(angle) * speed
        };
        smoke.startColor = sf::Color(120, 115, 105, 150);
        smoke.endColor = sf::Color(60, 60, 65, 0);
        smoke.lifetime = randomFloat(rng, 0.75f, 1.2f);
        smoke.spin = randomFloat(rng, -4.f, 4.f);
        particles.push_back(smoke);
    }

    for(int i = 0; i < 4; ++i)
    {
        ExplosionParticle flash;
        const float radius = 11.f + i * 5.f;
        flash.shape.setRadius(radius);
        flash.shape.setOrigin({radius, radius});
        flash.shape.setPosition(position);
        flash.shape.setFillColor(sf::Color(255, 245, 190, 210));
        flash.velocity = {0.f, 0.f};
        flash.startColor = sf::Color(255, 245, 190, static_cast<std::uint8_t>(210 - i * 40));
        flash.endColor = sf::Color(255, 110, 30, 0);
        flash.lifetime = 0.18f + i * 0.04f;
        particles.push_back(flash);
    }
}

void Explosion::update()
{
    constexpr float frameSeconds = 1.f / 60.f;

    for(auto& particle : particles)
    {
        particle.age += frameSeconds;

        const float progress = std::min(particle.age / particle.lifetime, 1.f);
        particle.shape.move(particle.velocity);
        particle.velocity *= 0.94f;
        particle.shape.scale({1.018f, 1.018f});
        particle.shape.rotate(sf::degrees(particle.spin));
        particle.shape.setFillColor(lerpColor(particle.startColor, particle.endColor, progress));
    }
}

void Explosion::draw(sf::RenderWindow& window) const
{
    for(const auto& particle : particles)
    {
        if(particle.age < particle.lifetime)
            window.draw(particle.shape);
    }
}

bool Explosion::isFinished() const
{
    for(const auto& particle : particles)
    {
        if(particle.age < particle.lifetime)
            return false;
    }

    return true;
}
