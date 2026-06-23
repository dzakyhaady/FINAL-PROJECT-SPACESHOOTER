#pragma once

#include "Background.hpp"
#include "Bullet.hpp"
#include "Explosion.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <string>
#include <vector>

enum class GameState
{
    MainMenu,
    Playing,
    GameOver
};

enum class BulletColorOption
{
    Basic,
    Blue,
    Red,
    Purple
};

enum class CortisolLevel
{
    Low,
    Medium,
    High
};

class Game
{
public:
    Game();

    int run();

private:
    bool loadResources();
    void processEvents();
    void update();
    void render();
    void renderGameObjects();
    void renderMainMenu();
    void renderGameOver();
    void renderDamageFlash();
    void drawText(const std::string& text, unsigned int size, sf::Vector2f position, sf::Color color);
    void drawTextWithFont(const std::string& text, unsigned int size, sf::Vector2f position, sf::Color color, const sf::Font& textFont, float outlineThickness = 0.f);
    void drawCenteredText(const std::string& text, unsigned int size, float y, sf::Color color);
    void drawCenteredTextWithFont(const std::string& text, unsigned int size, float y, sf::Color color, const sf::Font& textFont, float outlineThickness = 0.f);
    void drawCenteredSelectableText(const std::string& text, unsigned int size, float y, sf::Color color, bool selected, float outlineThickness = 0.f);

    void handleMainMenuInput();
    void handleGameOverInput();
    bool canReadMenuInput();
    void startGame();
    void returnToMenu();
    void cycleBulletColor();
    void cycleCortisol();
    void unlockAllBulletColors();
    void triggerGameOver();
    void updateBulletUnlockProgress();
    void playScoreMilestoneSound();
    void playDamageSound();
    void playGameOverSounds();

    void shoot();
    void spawnObstacle();
    void checkCollisions();
    bool hasPixelPerfectCollision(const Obstacle& obstacle) const;
    void cleanObjects();
    void restart();

    sf::Color getSelectedBulletColor() const;
    std::string getBulletColorName(BulletColorOption option) const;
    int getBulletColorRequiredScore(BulletColorOption option) const;
    bool isBulletColorUnlocked(BulletColorOption option) const;
    std::string getBulletUnlockText(BulletColorOption option) const;
    std::string getCortisolName() const;
    float getObstacleSpeed() const;
    float getSpawnDelay() const;
    float getShootDelay() const;

    sf::RenderWindow window;

    sf::Texture backgroundTexture;
    sf::Texture blurredBackgroundTexture;
    sf::Texture playerTexture;
    sf::Texture obstacleTexture;
    sf::Image playerImage;
    sf::Image obstacleImage;
    sf::Font font;
    sf::Font menuFont;
    sf::Font gameOverFont;
    sf::Music music;
    sf::SoundBuffer spongebobFailBuffer;
    sf::SoundBuffer fahhhBuffer;
    sf::SoundBuffer scoreMilestoneBuffer;
    sf::SoundBuffer damageBuffer;
    sf::Sound spongebobFailSound{spongebobFailBuffer};
    sf::Sound fahhhSound{fahhhBuffer};
    sf::Sound scoreMilestoneSound{scoreMilestoneBuffer};
    sf::Sound damageSound{damageBuffer};

    std::unique_ptr<Background> background;
    std::unique_ptr<Background> blurredBackground;
    std::unique_ptr<Player> player;

    std::vector<Bullet> bullets;
    std::vector<Obstacle> obstacles;
    std::vector<Explosion> explosions;

    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;

    sf::Clock shootClock;
    sf::Clock spawnClock;
    sf::Clock restartClock;
    sf::Clock menuInputClock;
    sf::Clock damageFlashClock;

    std::mt19937 rng;
    std::uniform_real_distribution<float> obstacleY;

    int score = 0;
    int nextScoreSoundMilestone = 1000;
    int highScore = 0;
    int health = 3;
    bool lowCompleted = false;
    bool mediumCompleted = false;
    bool highCompleted = false;
    bool damageFlashActive = false;
    int mainMenuSelection = 0;
    int gameOverSelection = 0;
    GameState state = GameState::MainMenu;
    BulletColorOption selectedBulletColor = BulletColorOption::Basic;
    CortisolLevel cortisol = CortisolLevel::Medium;
};
