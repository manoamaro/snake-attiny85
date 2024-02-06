#include <unity.h>
#include "snake.cpp"
#include <stdio.h>

void check_body_pos(uint8_t (*body)[2], int i, int x, int y) {
    auto str = new char[100];
    snprintf(str, 100, "X position of the snake's body %d", i);
    UNITY_TEST_ASSERT_EQUAL_UINT8(x, body[i][0], __LINE__, str);
    snprintf(str, 100, "Y position of the snake's body %d", i);
    UNITY_TEST_ASSERT_EQUAL_UINT8(y, body[i][1], __LINE__, str);
}

void test_snake_initial_state(void) {
    Snake snake;

    UNITY_TEST_ASSERT_EQUAL_UINT8(5, snake.getSize(), __LINE__, "Initial size of the snake should be 5");
    uint8_t (*body)[2] = snake.getBody();
    for (int i = 1; i < 5; i++) {
        check_body_pos(body, i, 10, 10 + i);
    }
}

void test_snake_move_up(void) {
    Snake snake;

    snake.changeDirection(SNAKE_UP);
    snake.move();

    uint8_t (*body)[2] = snake.getBody();
    for (int i = 0; i < 5; i++) {
        check_body_pos(body, i, 10, 10 + i - 1);
    }
}


void test_snake_move_right(void) {
    Snake snake;

    snake.changeDirection(SNAKE_RIGHT);
    snake.move();
    snake.move();
    snake.move();

    uint8_t (*body)[2] = snake.getBody();
    check_body_pos(body, 0, 13, 10);
    check_body_pos(body, 1, 12, 10);
    check_body_pos(body, 2, 11, 10);
    check_body_pos(body, 3, 10, 10);
    check_body_pos(body, 4, 10, 11);
}

void test_snake_move_left(void) {
    Snake snake;

    snake.changeDirection(SNAKE_LEFT);
    snake.move();
    snake.move();
    snake.move();

    uint8_t (*body)[2] = snake.getBody();
    check_body_pos(body, 0, 7, 10);
    check_body_pos(body, 1, 8, 10);
    check_body_pos(body, 2, 9, 10);
    check_body_pos(body, 3, 10, 10);
    check_body_pos(body, 4, 10, 11);
}

void test_snake_move_down(void) {
    Snake snake;

    snake.changeDirection(SNAKE_LEFT);
    snake.move();
    check_body_pos(snake.getBody(), 0, 9, 10);
    check_body_pos(snake.getBody(), 1, 10, 10);
    check_body_pos(snake.getBody(), 2, 10, 11);
    check_body_pos(snake.getBody(), 3, 10, 12);
    check_body_pos(snake.getBody(), 4, 10, 13);

    snake.changeDirection(SNAKE_DOWN);
    snake.move();
    check_body_pos(snake.getBody(), 0, 9, 11);
    check_body_pos(snake.getBody(), 1, 9, 10);
    check_body_pos(snake.getBody(), 2, 10, 10);
    check_body_pos(snake.getBody(), 3, 10, 11);
    check_body_pos(snake.getBody(), 4, 10, 12);

    snake.move();
    check_body_pos(snake.getBody(), 0, 9, 12);
    check_body_pos(snake.getBody(), 1, 9, 11);
    check_body_pos(snake.getBody(), 2, 9, 10);
    check_body_pos(snake.getBody(), 3, 10, 10);
    check_body_pos(snake.getBody(), 4, 10, 11);
}

void test_snake_grow(void) {
    Snake snake;

    snake.changeDirection(SNAKE_LEFT);
    snake.move();
    snake.move();
    check_body_pos(snake.getBody(), 0, 8, 10);
    check_body_pos(snake.getBody(), 1, 9, 10);
    check_body_pos(snake.getBody(), 2, 10, 10);
    check_body_pos(snake.getBody(), 3, 10, 11);
    check_body_pos(snake.getBody(), 4, 10, 12);

    snake.grow();
        UNITY_TEST_ASSERT_EQUAL_UINT8(6, snake.getSize(), __LINE__, "size of the snake should be 6");
    snake.move();
    check_body_pos(snake.getBody(), 0, 7, 10);
    check_body_pos(snake.getBody(), 1, 8, 10);
    check_body_pos(snake.getBody(), 2, 9, 10);
    check_body_pos(snake.getBody(), 3, 10, 10);
    check_body_pos(snake.getBody(), 4, 10, 11);
    check_body_pos(snake.getBody(), 5, 10, 12);
}

void setUp(void) {
    // Set up your testing objects here
}

void tearDown(void) {
    // Clean up your testing objects here
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_snake_initial_state);
    RUN_TEST(test_snake_move_up);
    RUN_TEST(test_snake_move_right);
    RUN_TEST(test_snake_move_left);
    RUN_TEST(test_snake_move_down);
    RUN_TEST(test_snake_grow);
    return UNITY_END();
}