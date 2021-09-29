#include <iostream>

#include "Stage.h"

CStage::CStage(SScreenInfo t_screen_info, LEVEL t_level) {
  grid_ = new CGrid{ t_screen_info.width_, t_screen_info.height_, t_screen_info.grid_size_ };
  background_ = new CBackground{ grid_, t_level };
  auto is_boundary = [&](UI32 d) -> bool {
    return ((d != 0) && (d != grid_->width() - 1) && (d != grid_->height() - 1));
  };
  food_ = new CFood{ this->random_point(is_boundary, is_boundary) };

  init_rest_points();
}

void CStage::init_rest_points() {
  for(UI32 id = 0; id < grid_->width() * grid_->height(); ++id) {
    if(grid_->at(id)->type() == POINT_TYPE::UNDEFINED) {
      grid_->at(id)->set_type(POINT_TYPE::SPACE);
    }
  }
}

void CStage::draw() const {
  this->grid_->draw();
}

void CStage::dump() const {
  grid_->dump();
}

CPoint* CStage::random_point() const {
  return grid_->at(RRND::Basic::random(0U, (grid_->width() * grid_->height()) - 1));
}

CPoint* CStage::random_point(std::function<bool(UI32)> t_x_constraint, std::function<bool(UI32)> t_y_constraint) const {
  CPoint* p = this->random_point();
  while(t_x_constraint(p->x()) == false || 
        t_y_constraint(p->y()) == false) {
    p = this->random_point();
  }
  return p;
}
