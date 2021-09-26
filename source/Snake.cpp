#include "Snake.h"

void CSnake::init(CGrid* t_grid) {
  this->length_ = static_cast<UI32>(DEFAULT::SNAKE_LENGTH);

  // random head position
  CPoint& p = t_grid->random_point();
  while(p.type() != POINT_TYPE::UNDEFINED) {
    p = t_grid->random_point();
  }
  p.set_type(POINT_TYPE::SNAKE_HEAD);
  this->head_ = &p;

  
}
