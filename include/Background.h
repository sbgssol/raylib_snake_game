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
  

protected:
  void init_wall();
  void init_obstacle();
private:
  CGrid* grid_;
  LEVEL level_ {LEVEL::_0};
  std::unordered_map<UI32, CPoint*> wall_;
  std::unordered_map<UI32, CPoint*> obstacle_;
};

