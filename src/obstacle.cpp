#include "obstacle.h"

// Constructor
Obstacle::Obstacle(sf::Texture& texture, int health)
    : texture(texture), speed(speed), health(health) {
    Init();
}

// Destructor
Obstacle::~Obstacle(){}

// Initialisation
void Obstacle::Init(){

    setTexture(texture);           // Player texture
    setScale(0.5f, 0.5f);          // Scale

    setRandomStartPosition();

    setRandomSpeed();
}

void Obstacle::updatePosition(sf::Time timeSinceLastframe) {
    float dy = timeSinceLastframe.asSeconds() * speed;
    move(0.0f, dy); // Move downward

    if (getPosition().y > WINDOW_HEIGHT) {
        setRandomStartPosition(); // Handle going off-screen
    }
}

void Obstacle::setRandomStartPosition() {
        // init random x position and y to sprite is just above the screen
        position.x = rand() % 1920;
        position.y = -getGlobalBounds().height;

        setPosition(position);
}

void Obstacle::setRandomSpeed() {
        // init random speed 
        speed = rand() % 200 + 50;
}

void Obstacle::takeDamage(int amount) {
    health -= amount;
}

bool Obstacle::isDestroyed() const {
    return health <= 0;
}
