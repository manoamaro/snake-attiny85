#include "snake.h"

Snake::Snake() {
    // Initialize the snake's body with the given size and starting position
    for (int i = 0; i < INITIAL_SNAKE_SIZE; i++) {
        body[i][0] = INITIAL_SNAKE_X;
        body[i][1] = INITIAL_SNAKE_Y + i;
    }
    // Set the initial direction to RIGHT
    direction = SNAKE_UP;
    size = INITIAL_SNAKE_SIZE;
    isChangingDirection = false;
}

const uint8_t Snake::getSize() const {
    // Return the current size of the snake
    return size;
}

const uint8_t (*Snake::getBody())[2] {
    // Return the pointer to the body array
    return body;  
}

const uint8_t (*Snake::getHead())[2] {
    // Return the pointer to the head position
    return &body[0];
}

void Snake::move() {
    // Get the current head position
    auto headX = body[0][0];
    auto headY = body[0][1];

    // Calculate the new head position based on the current direction
    auto newHeadX = headX;
    auto newHeadY = headY;
    switch (direction) {
        case SNAKE_UP:
            newHeadY--;
            break;
        case SNAKE_DOWN:
            newHeadY++;
            break;
        case SNAKE_LEFT:
            newHeadX--;
            break;
        case SNAKE_RIGHT:
            newHeadX++;
            break;
    }

    // insert the new head position at the beginning of the body array
    for (int i = size; i > 0; i--) {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }
    body[0][0] = newHeadX;
    body[0][1] = newHeadY;

    // remove the last element of the body array
    body[size][0] = 0;
    body[size][1] = 0;
    isChangingDirection = false;
}

void Snake::grow() {
    // increase the body of the snake by adding a new element at the end
    body[size][0] = body[size - 1][0];
    body[size][1] = body[size - 1][1];

    // Increase the size of the snake by one
    size++;
}

void Snake::changeDirection(uint8_t newDirection) {
    // If the snake is already changing direction, ignore the new direction
    if (isChangingDirection || newDirection == direction) {
        return;
    }
    // Update the direction of the snake. It cannot move in the opposite direction
    if (newDirection == SNAKE_UP && direction != SNAKE_DOWN) {
        direction = SNAKE_UP;
        isChangingDirection = true;
    } else if (newDirection == SNAKE_DOWN && direction != SNAKE_UP) {
        direction = SNAKE_DOWN;
        isChangingDirection = true;
    } else if (newDirection == SNAKE_LEFT && direction != SNAKE_RIGHT) {
        direction = SNAKE_LEFT;
        isChangingDirection = true;
    } else if (newDirection == SNAKE_RIGHT && direction != SNAKE_LEFT) {
        direction = SNAKE_RIGHT;
        isChangingDirection = true;
    }
}

bool Snake::isCollidingWithSelf() const {
    // Check if the head position is already present in the body vector
    auto headX = body[0][0];
    auto headY = body[0][1];
    for (int i = 1; i < size; i++) {
        if (headX == body[i][0] && headY == body[i][1]) {
            return true;
        }
    }
    return false;
}

bool Snake::isOutOfBounds(uint8_t width, uint8_t height) const {
    // Check if the head position is outside the boundaries
    auto headX = body[0][0];
    auto headY = body[0][1];
    return (headX < 0 || headX >= width || headY < 0 || headY >= height);
}

bool Snake::isCollidingWithFood(uint8_t foodX, uint8_t foodY) const {
    // Check if the head position is the same as the food position
    auto headX = body[0][0];
    auto headY = body[0][1];
    return (headX == foodX && headY == foodY);
}

bool Snake::isCollidingWithBody(uint8_t x, uint8_t y) const {
    // Check if the given position is already present in the body vector
    for (int i = 0; i < size; i++) {
        if (x == body[i][0] && y == body[i][1]) {
            return true;
        }
    }
    return false;
}
