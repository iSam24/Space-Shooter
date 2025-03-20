#include "gameManager.h"
#include <iostream>

gameManager::gameManager(): window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game"), player(), gameOver(false) {
    init();
    initSpawnObstacles();
}

void gameManager::init() {
    // Setup for game over text
    if (!gameFont.loadFromFile("assets/retro.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    gameOverText.setFont(gameFont);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(200);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 400, WINDOW_HEIGHT / 2 - 250);

    // Setup Score
    scoreText.setFont(gameFont);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);               // Make the text visible
    scoreText.setPosition(WINDOW_WIDTH - 200.f, 10.f);      // Position at top-right
    scoreText.setString("Score: 0");                        // Default text


    // Setup background
    if(backgroundTexture.loadFromFile("assets/background.png")) {
        std::cerr << "Failed to load background!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
}

void gameManager::run() {
    while (window.isOpen()) {
        sf::Time timeSinceLastFrame = clock.restart();
        processWindowEvents();
        if(!gameOver) {
            update(timeSinceLastFrame);
        }
        render();
    }
}

void gameManager::initSpawnObstacles() {
    textures["meteorBig"].loadFromFile("assets/meteorBig.png");
    textures["meteorSmall"].loadFromFile("assets/meteorSmall.png");

    obstacles.emplace_back(textures["meteorBig"], 10);   // large meteors have 3 health
    obstacles.emplace_back(textures["meteorBig"], 12);   // large meteors have 3 health
    obstacles.emplace_back(textures["meteorBig"], 8);   // large meteors have 3 health
    obstacles.emplace_back(textures["meteorSmall"], 3); // small meteors have 1 health
    obstacles.emplace_back(textures["meteorSmall"], 3); // small meteors have 1 health
}

void gameManager::spawnRandomObstcale() {
    int index = rand() % textures.size();                               // Get a random index within map size
    auto it = std::next(textures.begin(), index);                       // Move 'index' steps from the beginning
    sf::Texture& selectedTexture = it->second;                          // Access the texture
    int health = rand() % MAX_OBSTACLE_HEALTH + MIN_OBSTACLE_HEALTH;    // Get a random health
    obstacles.emplace_back(selectedTexture, health);
}

void gameManager::processWindowEvents() {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
}

void gameManager::update(sf::Time timeSinceLastFrame) {
    player.userInput(timeSinceLastFrame);
    player.updateBullets(timeSinceLastFrame);

    for(auto& obstacle : obstacles) {
        obstacle.updatePosition(timeSinceLastFrame);
    }

    checkCollisions();
    checkBulletCollisions();
    checkPlayerIsAlive();
    spawnObstacleManager();
    updateScore();
}

void gameManager::render() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(player);
    window.draw(scoreText);

    for(auto& bullet: player.getBullets()) {
        window.draw(bullet);
    }

    for(auto& obstacle: obstacles) {
        window.draw(obstacle);
    }

    if(gameOver) {
        scoreText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50);
        window.draw(scoreText);
        window.draw(gameOverText);
    }

    window.display();
}

// checks for collisions between player and obstacles
void gameManager:: checkCollisions() {
    if(collisionClock.getElapsedTime() < collisionCooldown){
        return; // Not enough time has passed
    }

    for(auto& obstacle : obstacles) {
        if(player.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
            std::cout << "Collision detected! Player Health: " << player.getHealth() << std::endl;
            // Deal 10 damage to the player
            player.takeDamage(40);
        }
    }
    collisionClock.restart();
}

void gameManager::checkPlayerIsAlive() {
    if(player.getHealth() <= 0) {
        gameOver = true;
    }
}

void gameManager::checkBulletCollisions() {
    auto& bullets = player.getBullets();  // Get reference to bullets

    for (size_t i = 0; i < bullets.size(); ++i) {  // Index-based loop to avoid iterator invalidation
        for (auto& obstacle : obstacles) {
            if (bullets[i].getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
                std::cout << "Bullet hit obstacle!" << std::endl;
                obstacle.takeDamage(1);    
                bullets[i].markForDeletion();    // Mark bullet for deletion

                if (obstacle.isDestroyed()) {
                    addScore(10);               // Award points when an obstacle is destroyed
                }
            }
        }
    }

    player.removeDeletedBullets();  // Remove bullets marked for deletion
    removeDestroyedObstacles();     // Remove destroyed obstacles 
}

void gameManager::removeDestroyedObstacles() {
    obstacles.remove_if([](Obstacle& obs) { return obs.isDestroyed(); });
}

void gameManager::updateObstacleSpawnInterval() {
    static sf::Time lastUpdateTime = sf::seconds(0);  // Store the last update time
    sf::Time elapsed = player.playerLifetime.getElapsedTime();

    // Only update every 5 seconds
    if (elapsed.asSeconds() - lastUpdateTime.asSeconds() >= 5) {
        obstacleSpawnInterval = std::max(obstacleSpawnInterval * 0.9f, minSpawnInterval);
        lastUpdateTime = elapsed;  // Update last update time
    }
}

void gameManager::spawnObstacleManager() {

    updateObstacleSpawnInterval(); // Adjust spawn frequency over time

    if(lastSpawnClock.getElapsedTime().asSeconds() >= obstacleSpawnInterval.asSeconds()) {
        spawnRandomObstcale();
        lastSpawnClock.restart();  // Reset timer after spawning
    }
}

void gameManager::updateScore() {

    scoreText.setString("Score: " + std::to_string(score));
}
