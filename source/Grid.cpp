#include <iostream>

#include "Grid.h"

//CPoint& CGrid::random_point() {
//  return grid_.at(rnd_.random());
//}
POINT_TYPE CGrid::initial_type_{ POINT_TYPE::UNDEFINED };
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

UI32 CGrid::maximum_point_id() const {
  return (height() * width() - 1);
}

void CGrid::calculate_heuristic_value(CPoint* t_food_position) {
  use_euclidean_distance(t_food_position);
  //use_manhattan_distance(t_food_position);
}

//TODO [DONE] Improve this function to increase FPS: background::draw = draw line only
void CGrid::draw() const {
  // horizontal lines
  for(UI32 x = 1; x < width(); ++x) {
    auto start_pos = Vector2{ static_cast<float>(0), static_cast<float>(x * grid_size()) };
    auto end_pos   = Vector2{ static_cast<float>(height()) * grid_size(),static_cast<float>(x * grid_size()) };
    DrawLineV(start_pos, end_pos, DARKGRAY);
  }

  // vertical lines
  for (UI32 y = 1; y <= height(); ++y) {
    auto start_pos = Vector2{ static_cast<float>(y * grid_size()), 0 };
    auto end_pos = Vector2{ static_cast<float>(y) * grid_size(),static_cast<float>(width() * grid_size()) };
    DrawLineV(start_pos, end_pos, DARKGRAY);
  }
}

void CGrid::draw_full() const {
  for (UI32 i = 0; i < linear_grid_.size(); ++i) {
    linear_grid_[i]->draw();
  }
}

CPoint* CGrid::at(UI32 t_id) {
  return &grid_.at(t_id);
}

CPoint const& CGrid::at(UI32 t_id) const {
  return grid_.at(t_id);
}

std::unordered_set<BOUNDARY_STATUS> CGrid::get_boundary_status(UI32 t_id) const {
  // BUG: point at corner have 2 status => Depend on direction to choose which boundary => DONE
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
    if (t_id == maximum_point_id()) {
      result.insert(BOUNDARY_STATUS::RIGHT);
    }
    return result;
  }
  if (t_id >= (this->height() * this->width() - this->height())) {
    std::unordered_set<BOUNDARY_STATUS> result {BOUNDARY_STATUS::RIGHT};
    if (t_id == this->height() * this->width() - this->height()) {
      result.insert(BOUNDARY_STATUS::TOP);
    }
    if (t_id == maximum_point_id()) {
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
      grid_.insert(std::make_pair(id, CPoint{ x, y, initial_type_ }));
      ; linear_grid_.push_back(&grid_.at(id));
    }
  }
}

void CGrid::use_manhattan_distance(CPoint* t_food_position) {
  for (auto& p : grid_) {
    if (p.second.is_reachable()) {
      float x_dis = std::abs(static_cast<SI32>(p.second.x()) - static_cast<SI32>(t_food_position->x()));
      float y_dis = std::abs(static_cast<SI32>(p.second.y()) - static_cast<SI32>(t_food_position->y()));
      p.second.heuristic_value_ = (x_dis + y_dis);
    }
  }
}

void CGrid::use_euclidean_distance(CPoint* t_food_position) {
  for(auto& p : grid_) {
    if(p.second.is_reachable()) {
      float x_dis = std::pow(static_cast<SI32>(p.second.x()) - static_cast<SI32>(t_food_position->x()), 2);
      float y_dis = std::pow(static_cast<SI32>(p.second.y()) - static_cast<SI32>(t_food_position->y()), 2);
      p.second.heuristic_value_ = std::sqrt(x_dis + y_dis);
    }
  }
}
