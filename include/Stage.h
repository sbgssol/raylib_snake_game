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

  void draw() const;
  void dump() const;
protected:
  CPoint* random_point() const;
  CPoint* random_point(std::function<bool(UI32)> t_x_constraint, std::function<bool(UI32)> t_y_constraint) const;
private:
  CGrid*               grid_;
  CBackground*         background_;
  CFood* food_;
  CSnake* snake_;
};

