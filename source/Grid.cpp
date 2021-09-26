#include "Grid.h"

CPoint& CGrid::random_point() {
  return grid_.at(rnd_.random());
}

void CGrid::init_grid() {
  CPoint::set_size(this->grid_size_);
  for(UI32 x = 0; x < width(); ++x) {
    for(UI32 y = 0; y < height(); ++y) {
      UI32 id = y * width() + x;
      grid_.insert({id, CPoint{x, y, POINT_TYPE::UNDEFINED}});
      rnd_.add(id);
    }
  }
}
