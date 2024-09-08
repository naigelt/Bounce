#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

/**
 * @brief Class representing a platform in the game.
 *
 * The platform is a rectangular object the player can land on.
 */
class Platform {
public:
    sf::RectangleShape shape; ///< The SFML shape representing the platform.

    /**
     * @brief Constructor for the platform.
     *
     * @param width Width of the platform.
     * @param height Height of the platform.
     * @param x X-coordinate of the platform.
     * @param y Y-coordinate of the platform.
     * @param color Color of the platform (default is green).
     */
    Platform(float width, float height, float x, float y, sf::Color color = sf::Color::Green);
};

/**
 * @brief Class representing a wall in the game.
 *
 * The wall is an obstacle the player or enemies cannot pass through.
 */
class Wall {
public:
    sf::RectangleShape shape; ///< The SFML shape representing the wall.

    /**
     * @brief Constructor for the wall.
     *
     * @param width Width of the wall.
     * @param height Height of the wall.
     * @param x X-coordinate of the wall.
     * @param y Y-coordinate of the wall.
     * @param color Color of the wall (default is blue).
     */
    Wall(float width, float height, float x, float y, sf::Color color = sf::Color::Blue);
};

/**
 * @brief Class representing a moving obstacle (enemy).
 *
 * The obstacle moves horizontally and reverses direction upon reaching movement boundaries.
 */
class Obstacle {
public:
    sf::RectangleShape shape; ///< The SFML shape representing the obstacle.
    float velocityX; ///< The horizontal velocity of the obstacle.
    float leftLimit, rightLimit; ///< Movement boundaries for the obstacle.

    /**
     * @brief Constructor for the obstacle.
     *
     * @param width Width of the obstacle.
     * @param height Height of the obstacle.
     * @param x X-coordinate of the obstacle.
     * @param y Y-coordinate of the obstacle.
     * @param leftLimit Left boundary of movement.
     * @param rightLimit Right boundary of movement.
     * @param color Color of the obstacle (default is red).
     */
    Obstacle(float width, float height, float x, float y, float leftLimit, float rightLimit, sf::Color color = sf::Color::Red);

    /**
     * @brief Moves the obstacle horizontally.
     *
     * Reverses direction when the obstacle reaches the left or right limits.
     */
    void move();

    /**
     * @brief Handles collisions with walls, reversing direction when hit.
     *
     * @param wall The wall the obstacle collides with.
     */
    void handleWallCollision(const Wall& wall);
};

/**
 * @brief Class representing a coin in the game.
 *
 * The coin can be collected by the player.
 */
class Coin {
public:
    sf::CircleShape shape; ///< The SFML shape representing the coin.

    /**
     * @brief Constructor for the coin.
     *
     * @param radius Radius of the coin.
     * @param x X-coordinate of the coin.
     * @param y Y-coordinate of the coin.
     */
    Coin(float radius, float x, float y);

    /**
     * @brief Checks if the player has collected the coin.
     *
     * @param player The player object.
     * @return true If the coin is collected.
     * @return false If the coin is not collected.
     */
    bool isCollected(const sf::CircleShape& player);
};

/**
 * @brief Resets the game state to its initial values.
 *
 * Resets the player's position, velocity, the level status, and repositions the coins.
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
void resetGame(sf::CircleShape& player, sf::Vector2f& velocity, bool& isOnGround, bool& levelCompleted, sf::View& view, int& coinCount, std::vector<Coin>& coins, const std::vector<sf::Vector2f>& defaultCoinPositions);
