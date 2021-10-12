#include <vector>
#include <iostream>

#include "Background.h"
#include "ratio_random.h"

void CBackground::draw() const {
  for(UI32 i = 0; i < wall_.size(); ++i) {
    wall_[i]->draw();
  }

  for (UI32 i = 0; i < obstacle_.size(); ++i) {
    obstacle_[i]->draw();
  }
}

void CBackground::init_wall() {
  wall_.clear();
  switch (level_) {
    case LEVEL::_0: {
      
    }
    //case LEVEL::_1: {
    //  for (UI32 id = 0; id < (grid_->width() * grid_->height()); ++id) {
    //    if (grid_->get_boundary_status(id) != IS_BOUNDARY::NONE) {
    //      grid_->at(id)->set_type(POINT_TYPE::WALL);
    //      wall_.insert(std::make_pair(id, grid_->at(id)));
    //    }
    //  }
    //}
    default:
      break;
  }
}

void CBackground::init_obstacle() {
  obstacle_.clear();
  UI32 maximum_point_id = grid_->maximum_point_id();
  switch (level_) {
    case LEVEL::_0: {
      for(UI32 i = 0; i < (UI32)OBSTACLE_NUM::LEVEL_0; ++i) {
        CPoint* p = grid_->at(RRND::Basic::random(0U, maximum_point_id));
        while (p->type() != CGrid::initial_type_) {
          p = grid_->at(RRND::Basic::random(0U, maximum_point_id));
        }
        p->set_type(POINT_TYPE::OBSTACLE);
        obstacle_.push_back(p);
      }
    }
    default:
      break;
  }
}
