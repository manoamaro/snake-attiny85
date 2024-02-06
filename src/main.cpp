#include <util/delay.h>
#include "utils.h"
#include "snake.h"
#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define WIDTH 128
#define HEIGHT 64

#define STATE_START 0
#define STATE_GAME 1
#define STATE_GAME_OVER 2

uint64_t _millis = 0;

uint8_t state = 0;

Snake snake;
uint8_t food[2] = {0, 0};
uint16_t score = 0;
uint8_t speed = 1;

void setupOLED(void)
{
  oled.begin(WIDTH, HEIGHT, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.setFont(FONT6X8);
  oled.clear();
  oled.on();
}

void drawSnake()
{
  auto body = snake.getBody();
  for (int i = 0; i < snake.getSize(); i++)
  {
    oled.setCursor(body[i][0], body[i][1]);
    oled.print("o");
  }
}

void drawFood()
{
  oled.setCursor(food[0], food[1]);
  oled.print("*");
}

void drawScore()
{
  oled.setCursor(0, 0);
  oled.print(F("Score: "));
  oled.print(score);
}

void drawGameOver()
{
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(F("Game Over!"));
  oled.setCursor(0, 10);
  oled.print(F("Score: "));
  oled.print(score);
  oled.setCursor(0, 20);
  oled.print(F("Speed: "));
  oled.print(speed);
  oled.setCursor(0, 30);
  oled.print(F("Press reset"));
  oled.setCursor(0, 40);
  oled.print(F("to play again"));
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
  // set 4 buttons as input. PB1, PB3, PB4, PB5
  DDRB &= ~((1 << PB1) | (1 << PB3) | (1 << PB4) | (1 << PB5));
  // enable pull-up resistors
  PORTB |= (1 << PB1) | (1 << PB3) | (1 << PB4) | (1 << PB5);
}

void readInput(void)
{
  // read the input from the buttons, changing the direction of the snake
  switch (state)
  {
  case STATE_START:
  case STATE_GAME_OVER:
    if (!(PINB & (1 << PB1)) || !(PINB & (1 << PB3)) || !(PINB & (1 << PB4)) || !(PINB & (1 << PB5))) {
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

void setupTimer0(void) {
  // setup timer to count milliseconds
  TCCR0A = 0; // set entire TCCR0A register to 0
  TCCR0B = 0; // same for TCCR0B
  TCNT0  = 0; // initialize counter value to 0
  // set compare match register for 1hz increments
  OCR0A = 249; // = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK |= (1 << OCIE0A);
}

// Timer 0 compare interrupt
ISR(TIMER0_COMPA_vect) {
  // increment the millis counter
  _millis++;
}

int main(void)
{
  setupOLED();
  setupButtons();
  setupTimer0();

  while (true)  
  {
    readInput();
    // renders at 24fps, 41ms per frame
    if (_millis < 41) {
      continue;
    }
    _millis = 0;
    if (state == 0)
    {
      oled.clear();
      oled.setCursor(0, 0);
      oled.print(F("Press any button"));
      oled.setCursor(0, 10);
      oled.print(F("to start"));
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
      oled.clear();
      drawSnake();
      drawFood();
      drawScore();
    }
    else if (state == 2)
    {
      drawGameOver();
    }
  }

  return 0;
}