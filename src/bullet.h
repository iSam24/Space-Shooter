#include <SFML/Graphics.hpp>
#include "constants.h"

class Bullet : public sf::Sprite {
    public:
        Bullet(sf::Texture& texture);   // Constructor
        void updatePosition(sf::Time timeSinceLastframe);
        void markForDeletion();
        static constexpr float speed = 500.0f;
        bool deletion = false;

    private:
        sf::Vector2f position;
};
