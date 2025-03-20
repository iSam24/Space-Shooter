#include <stdio.h>
#include <string>
#include <iostream>
#include <utility>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "constants.h"

class Obstacle : public sf::Sprite{
    public:
        Obstacle(sf::Texture& texture, int health); // constructor
        ~Obstacle(); // Destructor
        void Init();
        void updatePosition(sf::Time timeSinceLastframe);
        void setRandomStartPosition();
        void setRandomSpeed();
        void takeDamage(int amount);
        bool isDestroyed() const;

    private:
        sf::Texture& texture;
        sf::Vector2f position;
        int health;
        float speed;
};
