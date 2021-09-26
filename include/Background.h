#pragma once
#include <unordered_map>

#include "type_alias.h"
#include "Grid.h"

class CBackground {
public:
  CBackground() = default;
  CBackground(CGrid* t_grid, LEVEL t_level) : grid_(t_grid), level_(t_level) {
    init_wall(t_level);
    init_obstacle(t_level);
  }

protected:
  void init_wall(LEVEL t_level);
  void init_obstacle(LEVEL t_level);
private:
  CGrid* grid_;
  LEVEL level_;
  std::unordered_map<UI32, CPoint> wall_;
  std::unordered_map<UI32, CPoint> obstacle_;
};

