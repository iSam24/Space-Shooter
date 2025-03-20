#include <stdio.h>
#include <string>
#include <iostream>
#include <utility>
#include <list>
#include <map>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "player.h"
#include "obstacle.h"

class gameManager : public sf::Sprite {
    public:
        gameManager();                                      // Constructor      
        void run();                                         // Runs the game loop
        void addScore(int points) { score += points; }      // Adds points to score
        int getScore() const { return score; }              // Returns the score

    private:
        bool gameOver;
        Player player;
        std::list<Obstacle> obstacles;                          // Stores multiple obstacles
        std::map<std::string, sf::Texture> textures;            // Store textures
        sf::RenderWindow window;
        sf::Clock clock;
        sf::Clock collisionClock;                               // Clock for player-object collisions
        sf::Time collisionCooldown = sf::seconds(0.1f);         // 0.1s delay
        sf::Font gameFont;
        sf::Text gameOverText;
        sf::Text scoreText;
        sf::Time obstacleSpawnInterval = sf::seconds(3.0f);     // Time between obstacle spawns
        sf::Time minSpawnInterval = sf::seconds(0.1f);          // Minimum spawn interval
        sf::Clock lastSpawnClock;                               // Tracks time since last spawn
        sf::Texture backgroundTexture;                          // Game background texture
        sf::Sprite backgroundSprite;                            // Background sprite

        static constexpr int MIN_OBSTACLE_HEALTH = 3;
        static constexpr int MAX_OBSTACLE_HEALTH = 10;
        int score = 0;                                          // game score
    
        void init();                                  // init gameManager
        void initSpawnObstacles();                    // Spawns obstacles
        void spawnRandomObstcale();                   // Spawns an obstacle with random health and texture
        void processWindowEvents();                   // Proccess window events such as closing
        void update(sf::Time timeSinceLastFrame);     // Updates the game
        void render();                                // Render textures to the screen
        void checkCollisions();                       // Check for collisions between the player and obstacles
        void checkPlayerIsAlive();                    // Checks the player health is greater than zero (still alive)
        void checkBulletCollisions();                 // Checks if a bullet hits an obstcale
        void removeDestroyedObstacles();              // Removes destroyed obstacles from the obstacles vector
        void updateObstacleSpawnInterval();           // Updates obstacle spawn frequency
        void spawnObstacleManager();                  // Manages obstacle spawning
        void updateScore();                           // Updates score text

};
