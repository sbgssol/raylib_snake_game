#include <iostream>

#include "Grid.h"

//CPoint& CGrid::random_point() {
//  return grid_.at(rnd_.random());
//}

CGrid::CGrid(UI32 const t_width, UI32 const t_height, UI32 const t_grid_size)
  : width_(t_width / t_grid_size),
  height_(t_height / t_grid_size),
  grid_size_(t_grid_size) {
  init_grid();
}

UI32 CGrid::width() const {
  return width_;
}

UI32 CGrid::height() const {
  return height_;
}

UI32 CGrid::grid_size() const {
  return grid_size_;
}

void CGrid::draw() const {
  for (auto const& p : grid_) {
    p.second.draw();
  }
}

CPoint* CGrid::at(UI32 t_id) {
  return &grid_.at(t_id);
}

CPoint const& CGrid::at(UI32 t_id) const {
  return grid_.at(t_id);
}

std::unordered_set<BOUNDARY_STATUS> CGrid::get_boundary_status(UI32 t_id) const {
  // BUG: point at corner have 2 status => Depend on direction to choose which boundary
  if (t_id < this->height()           ) {
    std::unordered_set<BOUNDARY_STATUS> result { BOUNDARY_STATUS::LEFT };
    if (t_id == 0) {
      result.insert(BOUNDARY_STATUS::TOP);
    }
    if(t_id == this->height() - 1) {
      result.insert(BOUNDARY_STATUS::BOTTOM);
    }
    return result;
  }
  if ((t_id) % this->height() == 0    ) {
    std::unordered_set<BOUNDARY_STATUS> result {BOUNDARY_STATUS::TOP};
    if (t_id == 0) {
      result.insert(BOUNDARY_STATUS::LEFT);
    }
    if (t_id == this->height() * this->width() - this->height()) {
      result.insert(BOUNDARY_STATUS::RIGHT);
    }
    return result;
  }
  if ((t_id + 1) % this->height() == 0) {
    std::unordered_set<BOUNDARY_STATUS> result {BOUNDARY_STATUS::BOTTOM};
    if (t_id == 0) {
      result.insert(BOUNDARY_STATUS::LEFT);
    }
    if (t_id == this->height() * this->width() - 1) {
      result.insert(BOUNDARY_STATUS::RIGHT);
    }
    return result;
  }
  if (t_id >= (this->height() * this->width() - this->height())) {
    std::unordered_set<BOUNDARY_STATUS> result {BOUNDARY_STATUS::RIGHT};
    if (t_id == this->height() * this->width() - this->height()) {
      result.insert(BOUNDARY_STATUS::TOP);
    }
    if (t_id == this->height() * this->width() - 1) {
      result.insert(BOUNDARY_STATUS::BOTTOM);
    }
    return result;
  }
  return std::unordered_set<BOUNDARY_STATUS>{};
}

UI32 CGrid::get_point_id(CPoint* const& t_point) const {
  return t_point->x() * this->height() + t_point->y();
}

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
