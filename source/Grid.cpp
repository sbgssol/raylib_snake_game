#include <iostream>

#include "Grid.h"

//CPoint& CGrid::random_point() {
//  return grid_.at(rnd_.random());
//}

void CGrid::dump() const {
  for(auto const & p : grid_) {
    p.second.dump();
  }
}

void CGrid::init_grid() {
  //std::cout << __FUNCTION__ << " - width: " << this->width() << ", height: " << this->height() << '\n';
  CPoint::set_size(this->grid_size_);
  for (UI32 x = 0; x < this->width(); ++x) {
    for (UI32 y = 0; y < this->height(); ++y) {
      UI32 id = x * this->height() + y;
      //std::cout << __FUNCTION__ << " - (" << x << ", " << y << "): " << id << '\n';
      grid_.insert(std::make_pair(id, CPoint{ x, y, POINT_TYPE::UNDEFINED }));
    }
  }
}
