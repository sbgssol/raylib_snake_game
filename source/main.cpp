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
using Snake = struct Snake {
  Vector2 position;
  Vector2 size;
  Vector2 speed;
  Color color;
};

using Food = struct Food {
  Vector2 position;
  Vector2 size;
  bool active;
  Color color;
};

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static int screenWidth = 900;
static int screenHeight = 600;

static int framesCounter = 0;
static bool gameOver = false;
static bool pause = false;

static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0};
static bool allowMove = false;
static Vector2 offset = {0};
static int counterTail = 0;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);

static void UpdateGame(void);

static void DrawGame(void);

static void UnloadGame(void);

static void UpdateDrawFrame(void);

float GRID_SIZE = 10;
int FPS = 60;

DIRECTION d = DIRECTION::UP; // Temporary value
bool key_pressed = false;

CStage* stage;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  RRND::Basic::set_seed(std::random_device{}());
  //RRND::Basic::set_seed(0x63f37b69);
  screenHeight = 600;
  screenWidth = 900;
  GRID_SIZE = 40;

  //std::cout << "random seed: " << std::hex << RRND::Basic::get_seed() << '\n';
  InitWindow(screenWidth, screenHeight, "classic game: snake");

  InitGame();

  //SetTargetFPS(FPS);


  // Main game loop
  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }

  UnloadGame(); // Unload loaded data (textures, sounds, models...)

  CloseWindow(); // Close window and OpenGL context

  return 0;
}


//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void) {
  key_pressed = false;
  delete stage;
  stage = new CStage{
  {static_cast<UI32>(screenHeight), static_cast<UI32>(screenHeight), static_cast<UI32>(GRID_SIZE)}, LEVEL::_0
  };
  //stage->dump();
  framesCounter = 0;
  gameOver = false;
  pause = false;  
}

// Update game (one frame)
void UpdateGame(void) {
  if (!gameOver) {
    if (IsKeyPressed('P')) pause = !pause;

    if (!pause) {
      if (IsKeyPressed(KEY_R)) {
        gameOver = true;
      }
      if (IsKeyPressed(KEY_T)) {
        SetTargetFPS(120);
      }
      if (IsKeyPressed(KEY_N)) {
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
      if (stage->is_food_collided()) {
        stage->handle_food_collision();
      }

      if (true) {
        stage->expand_frontier();
      }
      // Snake movement
      if ((framesCounter % 500) == 0 && key_pressed) {
        // TODO: Control speed by framesCounter and modulo
        //stage->move_snake(d);

      }
      
      framesCounter++;
    }
  } else {
    if (IsKeyPressed(KEY_ENTER)) {
      InitGame();
      gameOver = false;
    }
  }
}

// Draw game (one frame)
void DrawGame(void) {
  BeginDrawing();

  ClearBackground(LIGHTGRAY);

  if (!gameOver) {
    stage->draw();
    auto fps = GetFPS();
    std::string fps_str = std::to_string(fps) + " FPS";
    DrawText(fps_str.c_str(), 30, 30, 30, RED);
    
    if (pause)
      DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40,
               GRAY);
  } else
    DrawText("PRESS [ENTER] TO PLAY AGAIN",
             GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50,
             20, GRAY);

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
