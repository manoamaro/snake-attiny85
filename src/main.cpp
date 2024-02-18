#define __AVR__ 1

#include <util/delay.h>
#include "utils.h"
#include "snake.h"
#include "lcdgfx.h"

#define WIDTH 128
#define HEIGHT 64

#define STATE_START 0
#define STATE_GAME 1
#define STATE_GAME_OVER 2

uint8_t state = 0;

Snake snake;
uint8_t food[2] = {0, 0};
uint16_t score = 0;
uint8_t speed = 1;

DisplaySSD1306_128x64_I2C display(-1); // or (-1,{busId, addr, scl, sda, frequency})

void setupOLED(void)
{
  display.begin();
  display.fill(0x00);
  display.setFixedFont(ssd1306xled_font6x8);
  display.printFixed(0, 8, "Snake", STYLE_NORMAL);
  lcd_delay(1000);
}

void drawSnake()
{
  auto body = snake.getBody();
  for (int i = 0; i < snake.getSize(); i++)
  {
    display.printFixed(body[i][0], body[i][1], "o", STYLE_NORMAL);
  }
}

void drawFood()
{
  display.printFixed(food[0], food[1], "*", STYLE_NORMAL);
}

void drawScore()
{
  display.setTextCursor(0, 0);
  display.write("Score: ");
  display.write(score);
}

void drawGameOver()
{
  display.clear();
  display.printFixed(0, 0, "Game Over", STYLE_NORMAL);
  display.printFixed(0, 10, "Score: ", STYLE_NORMAL);
  display.printFixed(0, 30, "Press any button", STYLE_NORMAL);
  state = 0;
}

void generateFood()
{
  // generate a new random position for the food, but make sure it's not colliding with the snake
  do
  {
    food[0] = random(0, WIDTH);
    food[1] = random(0, HEIGHT);
  } while (snake.isCollidingWithBody(food[0], food[1]));
}

void setupButtons(void)
{
  // set 4 buttons as input. PB0, PB1, PB2, PB5
  DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB5));
  // enable pull-up resistors
  PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB5);
}

void readInput(void)
{
  // read the input from the buttons, changing the direction of the snake
  switch (state)
  {
  case STATE_START:
  case STATE_GAME_OVER:
    if (!(PINB & (1 << PB1)) || !(PINB & (1 << PB3)) || !(PINB & (1 << PB4)) || !(PINB & (1 << PB5)))
    {
      snake = Snake();
      speed = 1;
      score = 0;
      state = STATE_GAME;
    }
    break;
  case STATE_GAME:
    if (!(PINB & (1 << PB1)))
    {
      snake.changeDirection(SNAKE_UP);
    }
    else if (!(PINB & (1 << PB3)))
    {
      snake.changeDirection(SNAKE_LEFT);
    }
    else if (!(PINB & (1 << PB4)))
    {
      snake.changeDirection(SNAKE_DOWN);
    }
    else if (!(PINB & (1 << PB5)))
    {
      snake.changeDirection(SNAKE_RIGHT);
    }
    break;
  default:
    break;
  }
}

void setup(void)
{
  setupOLED();
  setupButtons();
}

void loop(void)
{
  readInput();
  if (state == 0)
  {
    display.clear();
    display.printFixed(0, 0, "Press any button", STYLE_NORMAL);
  }
  else if (state == 1)
  {
    // Game loop
    snake.move();
    if (snake.isCollidingWithSelf() || snake.isOutOfBounds(WIDTH, HEIGHT))
    {
      state = 2;
    }
    else if (snake.isCollidingWithFood(food[0], food[1]))
    {
      snake.grow();
      score++;
      if (score % 5 == 0)
      {
        speed++;
      }
      generateFood();
    }
    display.clear();
    drawSnake();
    drawFood();
    drawScore();
  }
  else if (state == 2)
  {
    drawGameOver();
  }
}

int main(void)
{
  setup();
  while (1)
  {
    loop();
  }
  return 0;
}