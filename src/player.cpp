#include "player.h"

// Constructor
Player::Player(){
    Init();
}

// Destructor
Player::~Player() {}

// Initialises the player
void Player::Init() {
    if (!playerTextures[playerState::NORMAL].loadFromFile("assets/player.png")        ||
        !playerTextures[playerState::BANK_LEFT].loadFromFile("assets/playerleft.png") ||
        !playerTextures[playerState::BANK_RIGHT].loadFromFile("assets/playerRight.png"))  {
        std::cerr << "Failed to load player textures!" << std::endl;
        return;
    }

    if(!bulletTexture.loadFromFile("assets/laserRed.png")) {
        std::cerr << "Failed to load bullet texture!" << std::endl;
        return;
    }

    // Init player variables 
    speed = 750.0f;
    health = MAX_HEALTH;
    currentState = playerState::NORMAL;

    setTexture(playerTextures[currentState]);   // Player texture
    setScale(0.5f, 0.5f);                       // Scale
    setPosition(960, 540);                      // Starting position
}

// Takes user input from the keyboard and updates vRequestPlayerMovement vector
void Player::userInput(sf::Time timeSinceLastframe) {
    vRequestedPlayerMovement = {0.f, 0.f};  // Reset movement each frame

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
    {
        vRequestedPlayerMovement.y = -0.2;
        currentState = playerState::NORMAL;
        setTexture(playerTextures[currentState]);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
    {
        vRequestedPlayerMovement.y = 0.2;
        currentState = playerState::NORMAL;
        setTexture(playerTextures[currentState]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
    {
        vRequestedPlayerMovement.x = -0.2;
        currentState = playerState::BANK_LEFT;
        setTexture(playerTextures[currentState]);
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
    {
        vRequestedPlayerMovement.x = 0.2;
        currentState = playerState::BANK_RIGHT;
        setTexture(playerTextures[currentState]);
    } 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if(bulletCooldownClock.getElapsedTime().asSeconds() > bulletCoolDown)
        {
            // create a bullet at the head of the player
            bullets.emplace_back(bulletTexture);  // Pass the texture when creating the bullet
            bullets.back().setPosition(getPosition());  // Start the bullet at the player's position
            bulletCooldownClock.restart();  // Restart the clock
        }
    }

    float x_move = vRequestedPlayerMovement.x * timeSinceLastframe.asSeconds() * speed;
    float y_move = vRequestedPlayerMovement.y * timeSinceLastframe.asSeconds() * speed;

    // Calculate new position after movement
    sf::Vector2f newPos = getPosition() + sf::Vector2f(x_move, y_move);
    
    // check players position is within bounds
    if(checkBounds(newPos))
    {
        move(x_move, y_move);
    }
}

// Checks the bounds of the player
bool Player::checkBounds(sf::Vector2f newPos) {

    float left = newPos.x;
    float right = newPos.x + getGlobalBounds().width;
    float top = newPos.y;
    float bottom = newPos.y + getGlobalBounds().height;

    if(left >= 0 && right <= WINDOW_WIDTH && top >= 0 && bottom <= WINDOW_HEIGHT-70) 
    {
        return true;
    } else {
        return false;
    }
}

// Updates bullets position
void Player::updateBullets(sf::Time timeSinceLastFrame) {
    for (Bullet& bullet : bullets) {
        bullet.updatePosition(timeSinceLastFrame);
    }
    
    checkBulletBounds(); // Remove bullets that go off-screen
}

// Checks bullet is within bounds
void Player::checkBulletBounds() {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [] (const Bullet& bullet) {return bullet.getPosition().y < 0;}),
        bullets.end()
    );
}

std::vector<Bullet>& Player::getBullets() {
    return bullets;
}

void Player::takeDamage(int amount) {
    health -= amount;
}

void Player::heal(int amount) {
    if(health + amount < MAX_HEALTH){
        health += amount;
    }
}

int Player::getHealth() {
    return health;
}

void Player::removeDeletedBullets() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [] (const Bullet& bullet) {return bullet.deletion;}), bullets.end());
}
