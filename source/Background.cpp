#include <vector>
#include <iostream>

#include "Background.h"
#include "random.h"

void CBackground::init_wall(LEVEL t_level) {
  wall_.clear();
  switch (t_level) {
    case LEVEL::_0: {
      std::vector<UI32> walls;
      UI32 const max_x = grid_->width();
      UI32 const max_y = grid_->height();

      watch(max_x * max_y);
      
      for(UI32 id = 0; id < max_x * max_y; ++id) {
        if(id < max_x) {
          grid_->at(id).set_type(POINT_TYPE::WALL);
        }
        if(id % max_x == 0) {
          grid_->at(id).set_type(POINT_TYPE::WALL);
        }
        if((id + 1) % max_x == 0) {
          grid_->at(id).set_type(POINT_TYPE::WALL);
        }
        if(id / max_x >= max_y - 1) {
          grid_->at(id).set_type(POINT_TYPE::WALL);
        }
      }
    }
    default:
      break;
  }
}

void CBackground::init_obstacle(LEVEL t_level) {
  obstacle_.clear();
  switch (t_level) {
    case LEVEL::_0: {
      UI32 const max_x = grid_->width();
      UI32 const max_y = grid_->height();
      UI32 counter = 0;
      while(counter < static_cast<UI32>(OBSTACLE_NUM::LEVEL_0)) {
        CPoint& p = grid_->random_point();
        if(p.type() != POINT_TYPE::WALL) {
          p.set_type(POINT_TYPE::OBSTACLE);
          ++counter;
        }
      }
      
    }
    default:
      break;
  }
}
