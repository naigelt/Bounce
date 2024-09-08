#include "game.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// Implementations of the constructors and functions

/**
 * @brief Constructs a platform with given dimensions and color.
 *
 * @param width Width of the platform.
 * @param height Height of the platform.
 * @param x X-coordinate position.
 * @param y Y-coordinate position.
 * @param color Color of the platform (default is green).
 */
Platform::Platform(float width, float height, float x, float y, sf::Color color) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(color);
}

/**
 * @brief Constructs a wall with given dimensions and color.
 *
 * @param width Width of the wall.
 * @param height Height of the wall.
 * @param x X-coordinate position.
 * @param y Y-coordinate position.
 * @param color Color of the wall (default is blue).
 */
Wall::Wall(float width, float height, float x, float y, sf::Color color) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(color);
}

/**
 * @brief Constructs a moving obstacle with specified dimensions and movement limits.
 *
 * @param width Width of the obstacle.
 * @param height Height of the obstacle.
 * @param x X-coordinate starting position.
 * @param y Y-coordinate starting position.
 * @param leftLimit Left boundary for movement.
 * @param rightLimit Right boundary for movement.
 * @param color Color of the obstacle (default is red).
 */
Obstacle::Obstacle(float width, float height, float x, float y, float leftLimit, float rightLimit, sf::Color color)
    : velocityX(2.0f), leftLimit(leftLimit), rightLimit(rightLimit) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(color);
}

/**
 * @brief Moves the obstacle horizontally and reverses direction when boundaries are reached.
 */
void Obstacle::move() {
    shape.move(velocityX, 0);
    if (shape.getPosition().x <= leftLimit || shape.getPosition().x + shape.getSize().x >= rightLimit) {
        velocityX = -velocityX;
    }
}

/**
 * @brief Reverses obstacle direction upon colliding with a wall.
 *
 * @param wall The wall with which the obstacle collides.
 */
void Obstacle::handleWallCollision(const Wall& wall) {
    if (shape.getGlobalBounds().intersects(wall.shape.getGlobalBounds())) {
        velocityX = -velocityX;
    }
}

/**
 * @brief Constructs a coin at a specified position.
 *
 * @param radius Radius of the coin.
 * @param x X-coordinate position.
 * @param y Y-coordinate position.
 */
Coin::Coin(float radius, float x, float y) {
    shape.setRadius(radius);
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Yellow);
}

/**
 * @brief Checks if the player has collected the coin.
 *
 * @param player The player object.
 * @return true If the coin is collected.
 * @return false If the coin is not collected.
 */
bool Coin::isCollected(const sf::CircleShape& player) {
    return player.getGlobalBounds().intersects(shape.getGlobalBounds());
}

/**
 * @brief Resets the game state to the initial values.
 *
 * @param player The player (ball) object.
 * @param velocity Player's velocity vector.
 * @param isOnGround Boolean indicating if the player is on the ground.
 * @param levelCompleted Boolean indicating if the level is completed.
 * @param view The view (camera) of the game.
 * @param coinCount The number of coins collected.
 * @param coins The list of all coin objects.
 * @param defaultCoinPositions The default positions of coins in the game.
 */
void resetGame(sf::CircleShape& player, sf::Vector2f& velocity, bool& isOnGround, bool& levelCompleted, sf::View& view, int& coinCount, std::vector<Coin>& coins, const std::vector<sf::Vector2f>& defaultCoinPositions) {
    player.setPosition(400, 300);
    velocity = sf::Vector2f(0.0f, 0.0f);
    isOnGround = false;
    levelCompleted = false;
    coinCount = 0;
    view.setCenter(400, 300);
    coins.clear();
    for (const auto& pos : defaultCoinPositions) {
        coins.emplace_back(10.0f, pos.x, pos.y);
    }
}

/**
 * @brief The main game loop and logic for the Bounce Game.
 *
 * @return int Exit status of the game.
 */
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bounce Game");
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0, 0, 800, 600));

    sf::CircleShape player(20.0f);
    player.setFillColor(sf::Color::Red);
    player.setPosition(400, 300);

    float gravity = 0.5f;
    float jumpStrength = -12.0f;
    float groundFriction = 0.9f;
    sf::Vector2f velocity(0.0f, 0.0f);
    bool isOnGround = false;
    bool levelCompleted = false;

    std::vector<Platform> platforms = {
        Platform(200.0f, 20.0f, 100.0f, 550.0f),
        Platform(150.0f, 20.0f, 350.0f, 500.0f),
        Platform(200.0f, 20.0f, 600.0f, 400.0f),
        Platform(150.0f, 20.0f, 900.0f, 350.0f),
        Platform(250.0f, 20.0f, 1250.0f, 300.0f),
        Platform(200.0f, 20.0f, 1700.0f, 450.0f),
        Platform(200.0f, 20.0f, 2100.0f, 380.0f),
        Platform(150.0f, 20.0f, 2600.0f, 550.0f),
        Platform(200.0f, 20.0f, 3100.0f, 400.0f),
        Platform(150.0f, 20.0f, 3700.0f, 350.0f),
        Platform(200.0f, 20.0f, 4200.0f, 300.0f)
    };

    Platform goal(100.0f, 20.0f, 4700.0f, 250.0f, sf::Color::Yellow);
    Platform floor(9000.0f, 20.0f, 0.0f, 580.0f);

    std::vector<Wall> walls = {
        Wall(20.0f, 180.0f, 600.0f, 420.0f),
        Wall(20.0f, 200.0f, 1600.0f, 450.0f),
        Wall(20.0f, 200.0f, 3100.0f, 400.0f),
        Wall(20.0f, 200.0f, 3700.0f, 350.0f)
    };

    std::vector<Obstacle> obstacles = {
        Obstacle(50.0f, 50.0f, 800.0f, 530.0f, 700.0f, 1100.0f),
        Obstacle(50.0f, 50.0f, 1500.0f, 530.0f, 1400.0f, 1800.0f),
        Obstacle(50.0f, 50.0f, 2300.0f, 530.0f, 2200.0f, 2500.0f),
        Obstacle(50.0f, 50.0f, 3200.0f, 530.0f, 3100.0f, 3400.0f),
        Obstacle(50.0f, 50.0f, 4000.0f, 530.0f, 3900.0f, 4200.0f)
    };

    std::vector<sf::Vector2f> defaultCoinPositions = {
        sf::Vector2f(500.0f, 500.0f),
        sf::Vector2f(1200.0f, 400.0f),
        sf::Vector2f(2000.0f, 450.0f)
    };

    std::vector<Coin> coins;
    for (const auto& pos : defaultCoinPositions) {
        coins.emplace_back(10.0f, pos.x, pos.y);
    }

    int coinCount = 0;
    sf::Font font;
    if (!font.loadFromFile("C:/C++ Jatkokurssi/Bounce/Bounce/assets/Roboto-Black.ttf")) {
        return -1;
    }
    sf::Text coinText;
    coinText.setFont(font);
    coinText.setCharacterSize(24);
    coinText.setFillColor(sf::Color::White);
    coinText.setPosition(700, 10);

    sf::Texture victoryTexture;
    if (!victoryTexture.loadFromFile("C:/C++ Jatkokurssi/Bounce/Bounce/assets/Viktory.png")) {
        return -1;
    }
    sf::Sprite victorySprite;
    victorySprite.setTexture(victoryTexture);
    victorySprite.setPosition(400, 200);

    /**
     * @brief Main game loop that runs while the window is open.
     * Handles player movement, collision detection, and updates game state.
     */
    while (window.isOpen())
    {
        // Poll events from the window (such as closing or key presses)
        sf::Event event;
        while (window.pollEvent(event))
        {
            /**
             * @brief Close the window if the user clicks the close button.
             */
            if (event.type == sf::Event::Closed)
                window.close();

            /**
             * @brief Restart the game if the level is completed or the player dies.
             * Triggered by pressing the 'R' key.
             */
            if (levelCompleted || event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R)
                {
                    resetGame(player, velocity, isOnGround, levelCompleted, view, coinCount, coins, defaultCoinPositions);  // Reset the game and coins
                }
            }
        }

        /**
         * @brief Allow player movement and game progression if the level is not completed.
         */
        if (!levelCompleted)
        {
            /**
             * @brief Handle player movement input for left and right directions.
             * Applies friction if no movement key is pressed.
             */
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                velocity.x = -5.0f;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                velocity.x = 5.0f;
            else
                velocity.x *= groundFriction;

            /**
             * @brief Handle jumping if the player is on the ground.
             */
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isOnGround)
            {
                velocity.y = jumpStrength;
                isOnGround = false;
            }

            /**
             * @brief Apply gravity to pull the player down constantly.
             */
            velocity.y += gravity;

            /**
             * @brief Update player position by applying velocity.
             */
            player.move(velocity);

            /**
             * @brief Check for collision with platforms using a lambda function.
             * If the player collides with a platform, the player's position and velocity are adjusted.
             */
            auto platformCollision = [&player, &velocity, &isOnGround](Platform& platform) {
                if (player.getGlobalBounds().intersects(platform.shape.getGlobalBounds()))
                {
                    if (velocity.y > 0)  // Ball is falling
                    {
                        player.setPosition(player.getPosition().x, platform.shape.getPosition().y - player.getRadius() * 2);  // Correct position
                        velocity.y = -velocity.y * 0.7f;  // Bounce with 70% energy retained
                        isOnGround = true;
                    }
                }
                };
            std::for_each(platforms.begin(), platforms.end(), platformCollision);

            /**
             * @brief Check for collision with the floor.
             * If the player lands on the floor, the player's velocity and position are adjusted.
             */
            if (player.getGlobalBounds().intersects(floor.shape.getGlobalBounds()))
            {
                if (velocity.y > 0)  // Ball is falling
                {
                    player.setPosition(player.getPosition().x, floor.shape.getPosition().y - player.getRadius() * 2);  // Correct position
                    velocity.y = -velocity.y * 0.7f;  // Bounce with 70% energy retained
                    isOnGround = true;
                }
            }

            /**
             * @brief Check for collision with walls (for player) using a lambda function.
             * Adjusts the player's position and velocity if they collide with a wall.
             */
            auto wallCollision = [&player, &velocity](Wall& wall) {
                sf::FloatRect playerBounds = player.getGlobalBounds();
                sf::FloatRect wallBounds = wall.shape.getGlobalBounds();

                if (player.getGlobalBounds().intersects(wall.shape.getGlobalBounds()))
                {
                    if (velocity.x > 0 && playerBounds.left + playerBounds.width > wallBounds.left && playerBounds.left < wallBounds.left)
                    {
                        // Player hitting from the left
                        player.setPosition(wallBounds.left - playerBounds.width, player.getPosition().y);
                        velocity.x = 0;  // Stop horizontal movement
                    }
                    else if (velocity.x < 0 && playerBounds.left < wallBounds.left + wallBounds.width && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width)
                    {
                        // Player hitting from the right
                        player.setPosition(wallBounds.left + wallBounds.width, player.getPosition().y);
                        velocity.x = 0;  // Stop horizontal movement
                    }
                }
                };
            std::for_each(walls.begin(), walls.end(), wallCollision);

            /**
             * @brief Move the obstacles and check for collisions.
             * If an obstacle collides with a wall, it reverses direction.
             * If the player collides with an obstacle, the game is reset.
             */
            for (auto& obstacle : obstacles)
            {
                obstacle.move();  // Move the obstacle left and right

                // Handle wall collisions for enemies
                for (auto& wall : walls) {
                    obstacle.handleWallCollision(wall);  // Reverse direction if enemies hit a wall
                }

                // Handle collisions with obstacles (red blocks)
                sf::FloatRect playerBounds = player.getGlobalBounds();
                sf::FloatRect obstacleBounds = obstacle.shape.getGlobalBounds();

                if (playerBounds.intersects(obstacleBounds))
                {
                    resetGame(player, velocity, isOnGround, levelCompleted, view, coinCount, coins, defaultCoinPositions);
                }
            }

            /**
             * @brief Check for coin collection using a lambda function.
             * If the player collects a coin, it is removed from the game and the coin count is updated.
             */
            auto collectCoin = [&player, &coinCount](Coin& coin) {
                if (coin.isCollected(player)) {
                    coinCount++;  // Increase the coin count when a coin is collected
                    return true;
                }
                return false;
                };
            coins.erase(std::remove_if(coins.begin(), coins.end(), collectCoin), coins.end());

            /**
             * @brief Update the coin counter text to show the number of collected coins.
             */
            std::ostringstream ss;
            ss << "Coins: " << coinCount;
            coinText.setString(ss.str());

            /**
             * @brief Update the coin counter text position relative to the view.
             */
            sf::Vector2f viewCenter = view.getCenter();
            sf::Vector2f viewSize = view.getSize();
            coinText.setPosition(viewCenter.x - viewSize.x / 2 + 10, viewCenter.y - viewSize.y / 2 + 10);  // 10 pixels padding from the top-left corner

            /**
             * @brief Check for collision with the goal.
             * Only allow winning if all coins are collected.
             */
            if (player.getGlobalBounds().intersects(goal.shape.getGlobalBounds()) && coins.empty())
            {
                levelCompleted = true;  // The player wins the level
                victorySprite.setPosition(view.getCenter().x - 200, view.getCenter().y - 150);  // Adjust the position of the sprite
            }

            /**
             * @brief Update the view (camera) to follow the player if the level is not completed.
             */
            if (!levelCompleted) {
                view.setCenter(player.getPosition().x + 200, 300);  // Keep the view centered ahead of the player
            }
            window.setView(view);
        }

        /**
         * @brief Render the game elements on the screen.
         */
        window.clear(sf::Color::Cyan);

        window.draw(player);   // Draw the player
        for (auto& platform : platforms)
            window.draw(platform.shape);  // Draw all platforms
        window.draw(floor.shape);  // Draw the floor
        for (auto& wall : walls)
            window.draw(wall.shape);  // Draw all walls
        for (auto& obstacle : obstacles)
            window.draw(obstacle.shape);  // Draw all obstacles
        for (auto& coin : coins)
            window.draw(coin.shape);  // Draw all coins
        window.draw(goal.shape);  // Draw the goal
        window.draw(coinText);  // Draw the coin counter

        /**
         * @brief If the level is completed, draw the victory image.
         */
        if (levelCompleted)
        {
            window.draw(victorySprite);  // Draw the victory image
        }

        window.display();  // Display everything drawn to the window
    }


    return 0;
}
