#pragma once
#include <unordered_map>

#include "type_alias.h"
#include "Grid.h"

class CBackground {
public:
  CBackground() = default;
  CBackground(CGrid* t_grid, LEVEL t_level) : grid_(t_grid), level_(t_level) {
    init_wall();
    init_obstacle();
  }

  void draw() const;
  bool is_obstacle_collided(CPoint* t_point) const;

protected:
  void init_wall();
  void init_obstacle();
private:
  CGrid* grid_;
  LEVEL level_ {LEVEL::_0};
  std::vector<CPoint*> wall_;
  std::vector<CPoint*> obstacle_;
};

