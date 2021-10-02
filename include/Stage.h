#pragma once
#include <functional>

#include "Grid.h"
#include "Background.h"
#include "Snake.h"
#include "Food.h"

class CStage {
public:
  CStage() = default;
  CStage(SScreenInfo t_screen_info, LEVEL t_level);
  ~CStage();

  void move_snake(DIRECTION t_d);
  DIRECTION get_snake_direction() const;
  DIRECTION get_initialized_snake_direction() const;
  bool food_collision() const;
  void handle_food_collision();

  void draw() const;
  void dump() const;
protected:
  void    spawn_food();
  void    increase_snake_length();
  CSnake* init_snake();
  CFood*  init_food();
  void    init_rest_points();
  CPoint* random_point() const;
  CPoint* random_point(std::function<bool(UI32)> t_x_constraint, std::function<bool(UI32)> t_y_constraint) const;

  DIRECTION    initialized_direction_{ DIRECTION::UP };
  bool         just_eat_food_ {false};
private:
  CGrid*       grid_;
  CBackground* background_;
  CFood*       food_;
  CSnake*      snake_;
};

