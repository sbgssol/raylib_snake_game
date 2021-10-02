#include <iostream>
#include <random>

#include "raylib.h"
#include "ratio_random.h"
#include "Stage.h"
//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SNAKE_LENGTH   256
#define SQUARE_SIZE     20 // pixel

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Snake {
  Vector2 position;
  Vector2 size;
  Vector2 speed;
  Color color;
} Snake;

typedef struct Food {
  Vector2 position;
  Vector2 size;
  bool active;
  Color color;
} Food;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static int     screenWidth                 = 900;
static int     screenHeight                = 600;
                                           
static int     framesCounter               = 0;
static bool    gameOver                    = false;
static bool    pause                       = false;
                                           
static Food    fruit                       = { 0 };
static Snake   snake[SNAKE_LENGTH]         = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static bool    allowMove                   = false;
static Vector2 offset                      = { 0 };
static int     counterTail                 = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         
static void UpdateGame(void);       
static void DrawGame(void);         
static void UnloadGame(void);       
static void UpdateDrawFrame(void);  

float GRID_SIZE = 10;
int   FPS       = 60;

DIRECTION d = DIRECTION::UP; // Temporary value
bool key_pressed = false;

CStage* stage;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  RRND::Basic::set_seed(0x63f37b69);
  screenHeight = 600;
  screenWidth  = 900;
  GRID_SIZE    = 20;
  
  //std::cout << "random seed: " << std::hex << RRND::Basic::get_seed() << '\n';
  InitWindow(screenWidth, screenHeight, "classic game: snake");

  InitGame();
  
  SetTargetFPS(FPS);
  

  // Main game loop
  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }
  
  UnloadGame();         // Unload loaded data (textures, sounds, models...)

  CloseWindow();        // Close window and OpenGL context

  return 0;
}


//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void) {
  key_pressed = false;
  delete stage;
  stage = new CStage{ {static_cast<UI32>(screenHeight), static_cast<UI32>(screenHeight), static_cast<UI32>(GRID_SIZE)}, LEVEL::_0 };
  //stage->dump();
  framesCounter = 0;
  gameOver = false;
  pause = false;

  counterTail = 1;
  allowMove = false;

  offset.x = screenWidth % (int)GRID_SIZE;
  offset.y = screenHeight % (int)GRID_SIZE;

  for (int i = 0; i < SNAKE_LENGTH; i++)     {
    snake[i].position = Vector2{ offset.x / 2, offset.y / 2 };
    snake[i].size = Vector2{ GRID_SIZE, GRID_SIZE };
    snake[i].speed = Vector2{ GRID_SIZE, 0 };

    if (i == 0) snake[i].color = DARKBLUE;
    else snake[i].color = BLUE;
  }

  for (int i = 0; i < SNAKE_LENGTH; i++)     {
    snakePosition[i] = Vector2{ 0.0f, 0.0f };
  }

  fruit.size = Vector2{ GRID_SIZE, GRID_SIZE };
  fruit.color = SKYBLUE;
  fruit.active = false;
}
// Update game (one frame)
void UpdateGame(void) {
  if (!gameOver)     {
    if (IsKeyPressed('P')) pause = !pause;

    if (!pause) {
      
      // Player control
      /*if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove) {
        snake[0].speed = Vector2{ GRID_SIZE, 0 };
        allowMove = false;
        d = DIRECTION::RIGHT;
        key_pressed = true;
      }
      if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove) {
        snake[0].speed = Vector2{ -GRID_SIZE, 0 };
        allowMove = false;
        d = DIRECTION::LEFT;
        key_pressed = true;
      }
      if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove) {
        snake[0].speed = Vector2{ 0, -GRID_SIZE };
        allowMove = false;
        d = DIRECTION::UP;
        key_pressed = true;
      }
      if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove) {
        snake[0].speed = Vector2{ 0, GRID_SIZE };
        allowMove = false;
        d = DIRECTION::DOWN;
        key_pressed = true;
      }*/
      if (IsKeyPressed(KEY_R)) {
        SetTargetFPS(240);
      }
      if(IsKeyPressed(KEY_T)) {
        SetTargetFPS(120);
      }
      if(IsKeyPressed(KEY_N)) {
        SetTargetFPS(60);
      }
      if (IsKeyPressed(KEY_RIGHT)) {
        d = DIRECTION::RIGHT;
        key_pressed = true;
      }
      if (IsKeyPressed(KEY_LEFT)) {
        d = DIRECTION::LEFT;
        key_pressed = true;
      }
      if (IsKeyPressed(KEY_UP)) {
        d = DIRECTION::UP;
        key_pressed = true;
      }
      if (IsKeyPressed(KEY_DOWN)) {
        d = DIRECTION::DOWN;
        key_pressed = true;
      }
      if(stage->food_collision()) {
        stage->handle_food_collision();
      }

      // Snake movement
      if ((framesCounter % 5) == 0 && key_pressed) { // TODO: Control speed by framesCounter and modulo
        stage->move_snake(d);
      }

      //for (int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;
      /*if ((framesCounter % 5) == 0)             {
        for (int i = 0; i < counterTail; i++)                 {
          if (i == 0)                     {
            snake[0].position.x += snake[0].speed.x;
            snake[0].position.y += snake[0].speed.y;
            allowMove = true;
          }                     else snake[i].position = snakePosition[i - 1];
        }
      }*/

      // Wall behaviour
      if (((snake[0].position.x) > (screenWidth - offset.x)) ||
          ((snake[0].position.y) > (screenHeight - offset.y)) ||
          (snake[0].position.x < 0) || (snake[0].position.y < 0)) {
        gameOver = true;
      }

      // Collision with yourself
      for (int i = 1; i < counterTail; i++)             {
        if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y)) gameOver = true;
      }

      // Fruit position calculation
      if (!fruit.active)             {
        fruit.active = true;
        fruit.position = Vector2{ GetRandomValue(0, (screenWidth / GRID_SIZE) - 1) * GRID_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / GRID_SIZE) - 1) * GRID_SIZE + offset.y / 2 };

        for (int i = 0; i < counterTail; i++)                 {
          while ((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))                     {
            fruit.position = Vector2{ GetRandomValue(0, (screenWidth / GRID_SIZE) - 1) * GRID_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / GRID_SIZE) - 1) * GRID_SIZE + offset.y / 2 };
            i = 0;
          }
        }
      }

      // Collision
      if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) &&
          (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y))             {
        snake[counterTail].position = snakePosition[counterTail - 1];
        counterTail += 1;
        fruit.active = false;
      }

      framesCounter++;
    }
  } else {
    if (IsKeyPressed(KEY_ENTER))         {
      InitGame();
      gameOver = false;
    }
  }
}

// Draw game (one frame)
void DrawGame(void) {
  BeginDrawing();

  ClearBackground(DARKGRAY);

  if (!gameOver) {
    stage->draw();

    // Draw snake
    for (int i = 0; i < counterTail; i++) DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);

    // Draw fruit to pick
    DrawRectangleV(fruit.position, fruit.size, fruit.color);
    

    if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
  }         else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

  EndDrawing();
}

// Unload game variables
void UnloadGame(void) {
  // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void) {
  UpdateGame();
  DrawGame();
}