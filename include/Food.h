#pragma once
#include "Point.h"
class CFood {
public:
  CFood() = default;
  CFood(CPoint* t_pos) : position_(t_pos) {
    position_->set_type(POINT_TYPE::FOOD);    
  }

  CPoint* position() const;
private:
  CPoint* position_;
};

