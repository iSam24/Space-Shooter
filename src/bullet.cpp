#include "bullet.h"

Bullet::Bullet(sf::Texture& texture){
    setTexture(texture);
    setScale(0.5f, 0.5f);
}

void Bullet::updatePosition(sf::Time timeSinceLastframe) {
    float dy = timeSinceLastframe.asSeconds() * speed;
    move(0.0f, -dy); // Move upwards
}

void Bullet::markForDeletion() {
    deletion = true;
}
