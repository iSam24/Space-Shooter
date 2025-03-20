#include <stdio.h>
#include <utility>
#include <string>
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Bullet.h"
#include <set>

class Player: public sf::Sprite {
    public:
        Player();   // Constructor
        ~Player();  // Destructor

        // Enum for player textures 
        enum class playerState {NORMAL, BANK_LEFT, BANK_RIGHT};
        void userInput(sf::Time timeSinceLastframe);
        void checkBulletBounds();
        void updateBullets(sf::Time timeSinceLastFrame);
        void removeDeletedBullets();
        std::vector<Bullet>& getBullets();
        void Init();
        void takeDamage(int amount);
        void heal(int amount);
        int getHealth();

        static constexpr int MAX_HEALTH = 100;
        sf::Vector2f vRequestedPlayerMovement;
        sf::Clock playerLifetime;    

    private:
        int health;
        std::map<playerState, sf::Texture> playerTextures;
        sf::Clock   bulletCooldownClock;
        static constexpr float bulletCoolDown = 0.2f;
        sf::Texture bulletTexture;
        std::vector<Bullet> bullets;
        playerState currentState;
        bool checkBounds(sf::Vector2f newPos);
        float speed;
};

