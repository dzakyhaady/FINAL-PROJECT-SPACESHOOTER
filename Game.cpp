#include "Game.hpp"
#include "Constants.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>

Game::Game()
    : window(
          sf::VideoMode({
              static_cast<unsigned>(Constants::WindowWidth),
              static_cast<unsigned>(Constants::WindowHeight)
          }),
          "Space Shooter"
      ),
      scoreText(font),
      gameOverText(font),
      restartText(font),
      rng(std::random_device{}()),
      obstacleY(0.f, Constants::WindowHeight - 100.f)
{
    window.setFramerateLimit(60);
}

int Game::run()
{
    if(!loadResources())
        return -1;

    while(window.isOpen())
    {
        processEvents();
        update();
        render();
    }

    return 0;
}

bool Game::loadResources()
{
    if(!backgroundTexture.loadFromFile("assets/bgcc.png"))
    {
        std::cout << "Gagal load bgcc.png\n";
        return false;
    }

    if(!blurredBackgroundTexture.loadFromFile("assets/bgcc_blur.png"))
    {
        std::cout << "Gagal load bgcc_blur.png\n";
        return false;
    }

    if(!playerImage.loadFromFile("assets/player_ship_pixel_right.png") ||
       !playerTexture.loadFromImage(playerImage))
    {
        std::cout << "Gagal load player_ship_pixel_right.png\n";
        return false;
    }

    if(!obstacleImage.loadFromFile("assets/batu.gif") ||
       !obstacleTexture.loadFromImage(obstacleImage))
    {
        std::cout << "Gagal load batu.gif\n";
        return false;
    }

    if(music.openFromFile("assets/backsound.wav"))
    {
        music.setLooping(true);
        music.setVolume(100.f);
        music.play();
    }

    if(!spongebobFailBuffer.loadFromFile("assets/sounds/spongebob-fail.mp3"))
    {
        std::cout << "Gagal load spongebob-fail.mp3\n";
        return false;
    }

    if(!fahhhBuffer.loadFromFile("assets/sounds/fahhh_KcgAXfs.mp3"))
    {
        std::cout << "Gagal load fahhh_KcgAXfs.mp3\n";
        return false;
    }

    if(!scoreMilestoneBuffer.loadFromFile("assets/sounds/fears-to-fathom-notification-sound.mp3"))
    {
        std::cout << "Gagal load fears-to-fathom-notification-sound.mp3\n";
        return false;
    }

    if(!damageBuffer.loadFromFile("assets/sounds/bone-crack.mp3"))
    {
        std::cout << "Gagal load bone-crack.mp3\n";
        return false;
    }

    spongebobFailSound.setVolume(90.f);
    fahhhSound.setVolume(85.f);
    scoreMilestoneSound.setVolume(80.f);
    damageSound.setVolume(90.f);

    if(!font.openFromFile("assets/arial.ttf") &&
       !font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cout << "Font tidak ditemukan. Simpan arial.ttf di folder assets.\n";
        return false;
    }

    if(!menuFont.openFromFile("assets/fonts/PressStart2P-Regular.ttf"))
    {
        std::cout << "Gagal load PressStart2P-Regular.ttf\n";
        return false;
    }

    if(!gameOverFont.openFromFile("assets/fonts/devil_breeze/Devil Breeze Bold.ttf"))
    {
        std::cout << "Gagal load Devil Breeze Bold.ttf\n";
        return false;
    }

    background = std::make_unique<Background>(backgroundTexture);
    blurredBackground = std::make_unique<Background>(blurredBackgroundTexture);
    player = std::make_unique<Player>(playerTexture, playerImage);

    scoreText = sf::Text(font);
    scoreText.setCharacterSize(30);

    gameOverText = sf::Text(font);
    gameOverText.setCharacterSize(60);
    gameOverText.setString("GAME OVER");

    restartText = sf::Text(font);
    restartText.setCharacterSize(28);

    return true;
}

void Game::processEvents()
{
    while(auto event = window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update()
{
    background->update();
    blurredBackground->update();

    if(state == GameState::MainMenu)
    {
        handleMainMenuInput();
        return;
    }

    if(state == GameState::GameOver)
    {
        handleGameOverInput();
        return;
    }

    player->update();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
       shootClock.getElapsedTime().asMilliseconds() > getShootDelay())
    {
        shoot();
    }

    if(spawnClock.getElapsedTime().asSeconds() > getSpawnDelay())
    {
        spawnObstacle();
    }

    for(auto& bullet : bullets)
        bullet.update();

    for(auto& obstacle : obstacles)
        obstacle.update();

    checkCollisions();

    for(auto& explosion : explosions)
        explosion.update();

    cleanObjects();
}

void Game::render()
{
    window.clear();

    if(state == GameState::MainMenu)
    {
        blurredBackground->draw(window);
        renderMainMenu();
    }
    else if(state == GameState::GameOver)
    {
        blurredBackground->draw(window);
        renderGameOver();
    }
    else
    {
        background->draw(window);
        renderGameObjects();
        renderDamageFlash();
    }

    window.display();
}

void Game::renderGameObjects()
{
    for(const auto& bullet : bullets)
        bullet.draw(window);

    for(const auto& obstacle : obstacles)
        obstacle.draw(window);

    for(const auto& explosion : explosions)
        explosion.draw(window);

    player->draw(window);

    scoreText.setString("Score : " + std::to_string(score));
    scoreText.setPosition({20.f, 20.f});
    window.draw(scoreText);

    drawText("High Score : " + std::to_string(highScore), 22, {20.f, 58.f}, sf::Color::White);
    drawText("Health : " + std::to_string(health) + " / 3", 22, {20.f, 88.f}, sf::Color(255, 210, 210));
}

void Game::renderMainMenu()
{
    drawCenteredTextWithFont("SPACE SHOOTER", 54, 110.f, sf::Color::White, menuFont, 3.f);
    drawCenteredText("High Score : " + std::to_string(highScore), 20, 190.f, sf::Color(200, 220, 255));

    const sf::Color selectedColor(255, 230, 80);
    const sf::Color normalColor(220, 230, 245);
    const sf::Color lockedColor(130, 140, 155);

    drawCenteredSelectableText("Play", 24, 280.f, mainMenuSelection == 0 ? selectedColor : normalColor, mainMenuSelection == 0);

    drawCenteredSelectableText(
        "Bullet Colour : " + getBulletColorName(selectedBulletColor),
        24,
        335.f,
        mainMenuSelection == 1 ? selectedColor : normalColor,
        mainMenuSelection == 1
    );

    drawCenteredSelectableText(
        "Cortisol : " + getCortisolName(),
        24,
        390.f,
        mainMenuSelection == 2 ? selectedColor : normalColor,
        mainMenuSelection == 2
    );

    drawCenteredText("Unlock Bullet Colour:", 16, 475.f, sf::Color(200, 220, 255));
    drawCenteredText("Basic : unlocked", 15, 510.f, normalColor);
    drawCenteredText(getBulletUnlockText(BulletColorOption::Blue), 15, 538.f, isBulletColorUnlocked(BulletColorOption::Blue) ? normalColor : lockedColor);
    drawCenteredText(getBulletUnlockText(BulletColorOption::Red), 15, 566.f, isBulletColorUnlocked(BulletColorOption::Red) ? normalColor : lockedColor);
    drawCenteredText(getBulletUnlockText(BulletColorOption::Purple), 15, 594.f, isBulletColorUnlocked(BulletColorOption::Purple) ? normalColor : lockedColor);

    drawCenteredText("W/S: pilih   Enter: gunakan", 14, 655.f, sf::Color(190, 200, 215));
}

void Game::renderGameOver()
{
    sf::RectangleShape overlay({Constants::WindowWidth, Constants::WindowHeight});
    overlay.setFillColor(sf::Color(15, 0, 0, 175));
    window.draw(overlay);

    sf::RectangleShape redVignette({Constants::WindowWidth, Constants::WindowHeight});
    redVignette.setFillColor(sf::Color(95, 0, 10, 70));
    window.draw(redVignette);

    drawCenteredTextWithFont("GAME OVER", 82, 190.f, sf::Color(255, 235, 235), gameOverFont, 5.f);
    drawCenteredTextWithFont("Score : " + std::to_string(score), 22, 292.f, sf::Color(255, 230, 80), menuFont, 1.f);
    drawCenteredTextWithFont("High Score : " + std::to_string(highScore), 18, 338.f, sf::Color(220, 225, 255), menuFont, 1.f);

    const sf::Color selectedColor(255, 230, 80);
    const sf::Color normalColor(220, 230, 245);
    drawCenteredSelectableText("Retry Game", 24, 430.f, gameOverSelection == 0 ? selectedColor : normalColor, gameOverSelection == 0, 1.f);
    drawCenteredSelectableText("Back to Menu", 24, 486.f, gameOverSelection == 1 ? selectedColor : normalColor, gameOverSelection == 1, 1.f);
    drawCenteredTextWithFont("W/S: pilih   Enter: pilih   R: retry   M: menu", 14, 604.f, sf::Color(205, 195, 205), menuFont);
}

void Game::renderDamageFlash()
{
    if(!damageFlashActive)
        return;

    const float duration = 0.45f;
    const float elapsed = damageFlashClock.getElapsedTime().asSeconds();

    if(elapsed >= duration)
    {
        damageFlashActive = false;
        return;
    }

    const float fade = 1.f - (elapsed / duration);
    const std::uint8_t alpha = static_cast<std::uint8_t>(190.f * fade);

    sf::RectangleShape frame({Constants::WindowWidth - 18.f, Constants::WindowHeight - 18.f});
    frame.setPosition({9.f, 9.f});
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineThickness(9.f);
    frame.setOutlineColor(sf::Color(255, 40, 40, alpha));
    window.draw(frame);
}

void Game::drawText(const std::string& text, unsigned int size, sf::Vector2f position, sf::Color color)
{
    drawTextWithFont(text, size, position, color, font);
}

void Game::drawTextWithFont(const std::string& text, unsigned int size, sf::Vector2f position, sf::Color color, const sf::Font& textFont, float outlineThickness)
{
    sf::Text label(textFont);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    label.setOutlineColor(sf::Color(20, 22, 35));
    label.setOutlineThickness(outlineThickness);
    label.setPosition(position);
    window.draw(label);
}

void Game::drawCenteredText(const std::string& text, unsigned int size, float y, sf::Color color)
{
    drawCenteredTextWithFont(text, size, y, color, menuFont);
}

void Game::drawCenteredTextWithFont(const std::string& text, unsigned int size, float y, sf::Color color, const sf::Font& textFont, float outlineThickness)
{
    sf::Text label(textFont);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    label.setOutlineColor(sf::Color(20, 22, 35));
    label.setOutlineThickness(outlineThickness);

    const auto bounds = label.getLocalBounds();
    label.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y
    });
    label.setPosition({Constants::WindowWidth / 2.f, y});
    window.draw(label);
}

void Game::drawCenteredSelectableText(const std::string& text, unsigned int size, float y, sf::Color color, bool selected, float outlineThickness)
{
    sf::Text label(menuFont);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    label.setOutlineColor(sf::Color(20, 22, 35));
    label.setOutlineThickness(outlineThickness);

    const auto bounds = label.getLocalBounds();
    label.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y
    });
    label.setPosition({Constants::WindowWidth / 2.f, y});
    window.draw(label);

    if(!selected)
        return;

    sf::Text marker(menuFont);
    marker.setString(">");
    marker.setCharacterSize(size);
    marker.setFillColor(color);
    marker.setOutlineColor(sf::Color(20, 22, 35));
    marker.setOutlineThickness(outlineThickness);

    const auto markerBounds = marker.getLocalBounds();
    marker.setOrigin({
        markerBounds.position.x + markerBounds.size.x,
        markerBounds.position.y
    });
    marker.setPosition({
        Constants::WindowWidth / 2.f - bounds.size.x / 2.f - 28.f,
        y
    });
    window.draw(marker);
}

void Game::handleMainMenuInput()
{
    if(!canReadMenuInput())
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) &&
       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U))
    {
        unlockAllBulletColors();
        menuInputClock.restart();
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        mainMenuSelection = (mainMenuSelection + 2) % 3;
        menuInputClock.restart();
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        mainMenuSelection = (mainMenuSelection + 1) % 3;
        menuInputClock.restart();
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
    {
        if(mainMenuSelection == 0)
            startGame();
        else if(mainMenuSelection == 1)
            cycleBulletColor();
        else
            cycleCortisol();

        menuInputClock.restart();
    }
}

void Game::handleGameOverInput()
{
    if(!canReadMenuInput())
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
       sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        gameOverSelection = 1 - gameOverSelection;
        menuInputClock.restart();
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
    {
        restart();
        menuInputClock.restart();
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))
    {
        returnToMenu();
        menuInputClock.restart();
        return;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
    {
        if(gameOverSelection == 0)
            restart();
        else
            returnToMenu();

        menuInputClock.restart();
    }
}

bool Game::canReadMenuInput()
{
    return menuInputClock.getElapsedTime().asMilliseconds() > 180;
}

void Game::startGame()
{
    bullets.clear();
    obstacles.clear();
    explosions.clear();
    score = 0;
    nextScoreSoundMilestone = 1000;
    health = 3;
    state = GameState::Playing;
    damageFlashActive = false;
    music.setVolume(100.f);
    player->reset();
    shootClock.restart();
    spawnClock.restart();
    restartClock.restart();
}

void Game::returnToMenu()
{
    bullets.clear();
    obstacles.clear();
    explosions.clear();
    score = 0;
    nextScoreSoundMilestone = 1000;
    health = 3;
    state = GameState::MainMenu;
    damageFlashActive = false;
    music.setVolume(100.f);
    player->reset();
}

void Game::cycleBulletColor()
{
    BulletColorOption options[] = {
        BulletColorOption::Basic,
        BulletColorOption::Blue,
        BulletColorOption::Red,
        BulletColorOption::Purple
    };

    int currentIndex = 0;

    for(int i = 0; i < 4; ++i)
    {
        if(options[i] == selectedBulletColor)
        {
            currentIndex = i;
            break;
        }
    }

    for(int step = 1; step <= 4; ++step)
    {
        BulletColorOption next = options[(currentIndex + step) % 4];

        if(isBulletColorUnlocked(next))
        {
            selectedBulletColor = next;
            return;
        }
    }
}

void Game::cycleCortisol()
{
    if(cortisol == CortisolLevel::Low)
        cortisol = CortisolLevel::Medium;
    else if(cortisol == CortisolLevel::Medium)
        cortisol = CortisolLevel::High;
    else
        cortisol = CortisolLevel::Low;
}

void Game::unlockAllBulletColors()
{
    lowCompleted = true;
    mediumCompleted = true;
    highCompleted = true;
    selectedBulletColor = BulletColorOption::Purple;
}

void Game::triggerGameOver()
{
    if(state == GameState::GameOver)
        return;

    highScore = std::max(highScore, score);
    state = GameState::GameOver;
    gameOverSelection = 0;
    playGameOverSounds();
    menuInputClock.restart();
}

void Game::updateBulletUnlockProgress()
{
    if(score >= 1000)
        lowCompleted = true;

    if(cortisol != CortisolLevel::Low && score >= 5000)
        mediumCompleted = true;

    if(cortisol == CortisolLevel::High && score >= 10000)
        highCompleted = true;
}

void Game::playScoreMilestoneSound()
{
    if(score < nextScoreSoundMilestone)
        return;

    scoreMilestoneSound.play();

    while(score >= nextScoreSoundMilestone)
        nextScoreSoundMilestone += 1000;
}

void Game::playDamageSound()
{
    damageSound.play();
}

void Game::playGameOverSounds()
{
    music.setVolume(55.f);
    spongebobFailSound.play();
    fahhhSound.play();
}

void Game::shoot()
{
    bullets.emplace_back(player->getBulletStartPosition(), getSelectedBulletColor());
    shootClock.restart();
}

void Game::spawnObstacle()
{
    obstacles.emplace_back(obstacleTexture, obstacleImage, obstacleY(rng), getObstacleSpeed());
    spawnClock.restart();
}

void Game::checkCollisions()
{
    for(auto& bullet : bullets)
    {
        if(!bullet.isActive())
            continue;

        for(auto& obstacle : obstacles)
        {
            if(!obstacle.isActive())
                continue;

            if(bullet.getBounds().findIntersection(obstacle.getBounds()).has_value() &&
               bullet.overlapsOpaquePixel(obstacle))
            {
                bullet.registerHit();
                obstacle.deactivate();
                score += 10;
                playScoreMilestoneSound();
                highScore = std::max(highScore, score);
                updateBulletUnlockProgress();
                explosions.emplace_back(obstacle.getCenter());

                if(!bullet.isActive())
                    break;
            }
        }
    }

    for(auto& obstacle : obstacles)
    {
        if(obstacle.isActive() && hasPixelPerfectCollision(obstacle))
        {
            triggerGameOver();
        }
    }
}

bool Game::hasPixelPerfectCollision(const Obstacle& obstacle) const
{
    auto intersection = player->getBounds().findIntersection(obstacle.getBounds());

    if(!intersection.has_value())
        return false;

    const auto overlap = intersection.value();
    const int left = static_cast<int>(std::floor(overlap.position.x));
    const int top = static_cast<int>(std::floor(overlap.position.y));
    const int right = static_cast<int>(std::ceil(overlap.position.x + overlap.size.x));
    const int bottom = static_cast<int>(std::ceil(overlap.position.y + overlap.size.y));

    for(int y = top; y < bottom; y += 2)
    {
        for(int x = left; x < right; x += 2)
        {
            sf::Vector2f point{
                static_cast<float>(x),
                static_cast<float>(y)
            };

            if(player->containsOpaquePixel(point) &&
               obstacle.containsOpaquePixel(point))
            {
                return true;
            }
        }
    }

    return false;
}

void Game::cleanObjects()
{
    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](const Bullet& bullet)
            {
                return !bullet.isActive() || bullet.isOffscreen();
            }
        ),
        bullets.end()
    );

    for(auto& obstacle : obstacles)
    {
        if(obstacle.isActive() && obstacle.isOffscreen())
        {
            obstacle.deactivate();
            health--;
            playDamageSound();
            damageFlashActive = true;
            damageFlashClock.restart();

            if(health <= 0)
                triggerGameOver();
        }
    }

    obstacles.erase(
        std::remove_if(
            obstacles.begin(),
            obstacles.end(),
            [](const Obstacle& obstacle)
            {
                return !obstacle.isActive() || obstacle.isOffscreen();
            }
        ),
        obstacles.end()
    );

    explosions.erase(
        std::remove_if(
            explosions.begin(),
            explosions.end(),
            [](const Explosion& explosion)
            {
                return explosion.isFinished();
            }
        ),
        explosions.end()
    );
}

void Game::restart()
{
    startGame();
}

sf::Color Game::getSelectedBulletColor() const
{
    if(selectedBulletColor == BulletColorOption::Blue)
        return sf::Color(80, 170, 255);

    if(selectedBulletColor == BulletColorOption::Red)
        return sf::Color(255, 80, 80);

    if(selectedBulletColor == BulletColorOption::Purple)
        return sf::Color(190, 90, 255);

    return sf::Color::Yellow;
}

std::string Game::getBulletColorName(BulletColorOption option) const
{
    if(option == BulletColorOption::Blue)
        return "Blue";

    if(option == BulletColorOption::Red)
        return "Red";

    if(option == BulletColorOption::Purple)
        return "Purple";

    return "Basic";
}

int Game::getBulletColorRequiredScore(BulletColorOption option) const
{
    if(option == BulletColorOption::Blue)
        return 1000;

    if(option == BulletColorOption::Red)
        return 5000;

    if(option == BulletColorOption::Purple)
        return 10000;

    return 0;
}

bool Game::isBulletColorUnlocked(BulletColorOption option) const
{
    if(option == BulletColorOption::Blue)
        return lowCompleted;

    if(option == BulletColorOption::Red)
        return mediumCompleted;

    if(option == BulletColorOption::Purple)
        return highCompleted;

    return true;
}

std::string Game::getBulletUnlockText(BulletColorOption option) const
{
    if(option == BulletColorOption::Blue)
        return lowCompleted ? "Blue  : unlocked" : "Blue  : reach 1000 on Low+";

    if(option == BulletColorOption::Red)
        return mediumCompleted ? "Red   : unlocked" : "Red   : reach 5000 on Medium+";

    if(option == BulletColorOption::Purple)
        return highCompleted ? "Purple: unlocked" : "Purple: reach 10000 on High";

    return "Basic : unlocked";
}

std::string Game::getCortisolName() const
{
    if(cortisol == CortisolLevel::Low)
        return "Low";

    if(cortisol == CortisolLevel::High)
        return "High";

    return "Medium";
}

float Game::getObstacleSpeed() const
{
    if(cortisol == CortisolLevel::Low)
        return 3.f;

    if(cortisol == CortisolLevel::High)
        return 9.f;

    return 5.5f;
}

float Game::getSpawnDelay() const
{
    if(cortisol == CortisolLevel::Low)
        return 1.8f;

    if(cortisol == CortisolLevel::High)
        return 0.45f;

    return 1.f;
}

float Game::getShootDelay() const
{
    if(selectedBulletColor == BulletColorOption::Blue)
        return 280.f;

    if(selectedBulletColor == BulletColorOption::Purple)
        return 380.f;

    return Constants::ShootDelayMs;
}
