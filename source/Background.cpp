#include <vector>
#include <iostream>

#include "Background.h"
#include "ratio_random.h"

void CBackground::init_wall() {
  wall_.clear();
  switch (level_) {
    case LEVEL::_0: {
      for (UI32 id = 0; id < (grid_->width() * grid_->height()); ++id) {
        if(id < grid_->height()) {
          grid_->at(id)->set_type(POINT_TYPE::WALL);
          wall_.insert(std::make_pair(id, grid_->at(id)));
        }
        if ((id) % grid_->height() == 0) {
          watch(id);
          grid_->at(id)->set_type(POINT_TYPE::WALL);
          wall_.insert(std::make_pair(id, grid_->at(id)));
        }
        if ((id + 1) % grid_->height() == 0) {
          grid_->at(id)->set_type(POINT_TYPE::WALL);
          wall_.insert(std::make_pair(id, grid_->at(id)));
        }
        if (id > (grid_->height() * grid_->width() - grid_->height())) {
          grid_->at(id)->set_type(POINT_TYPE::WALL);
          wall_.insert(std::make_pair(id, grid_->at(id)));
        }
      }
    }
    default:
      break;
  }
}

void CBackground::init_obstacle() {
  obstacle_.clear();
  switch (level_) {
    case LEVEL::_0: {
    }
    default:
      break;
  }
}
