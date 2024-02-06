#ifndef SNAKE_H
#define SNAKE_H

#include "inttypes.h"

#define MAX_SNAKE_SIZE 150
#define INITIAL_SNAKE_SIZE 5
#define INITIAL_SNAKE_X 10
#define INITIAL_SNAKE_Y 10
#define SNAKE_UP 0
#define SNAKE_DOWN 1
#define SNAKE_LEFT 2
#define SNAKE_RIGHT 3

class Snake {
public:
    explicit Snake();
    void move();
    void grow();
    const uint8_t getSize() const;
    const uint8_t (*getHead())[2];
    void changeDirection(uint8_t newDirection);
    bool isCollidingWithSelf() const;
    bool isOutOfBounds(uint8_t width, uint8_t height) const;
    bool isCollidingWithFood(uint8_t foodX, uint8_t foodY) const;
    bool isCollidingWithBody(uint8_t x, uint8_t y) const;
    const uint8_t (*getBody())[2];
private:
    uint8_t body[MAX_SNAKE_SIZE][2];
    uint8_t direction;
    bool isChangingDirection;
    int size;
};

#endif

