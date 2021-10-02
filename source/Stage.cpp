#include <iostream>

#include "Stage.h"
#include "ratio_random.h"

CStage::CStage(SScreenInfo t_screen_info, LEVEL t_level) {
  grid_ = new CGrid{ t_screen_info.width_, t_screen_info.height_, t_screen_info.grid_size_ };
  background_ = new CBackground{ grid_, t_level };
  food_ = init_food();
  snake_ = init_snake();

  init_rest_points();
}

CStage::~CStage() {
  delete grid_;
  delete background_;
  delete food_;
  delete snake_;
}

void CStage::move_snake(DIRECTION t_d) {
  this->snake_->move(t_d);
}

DIRECTION CStage::get_snake_direction() const {
  return snake_->get_direction();
}

DIRECTION CStage::get_initialized_snake_direction() const {
  return initialized_direction_;
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

CSnake* CStage::init_snake() {
  // random head;
  auto far_from_wall = [&](UI32 d) -> bool {
    return ((d > static_cast<UI32>(DEFAULT::SNAKE_LENGTH)) && 
            (d < grid_->width() - 1 - static_cast<UI32>(DEFAULT::SNAKE_LENGTH) && (d < grid_->height() - 1 - static_cast<UI32>(DEFAULT::SNAKE_LENGTH))));
  };
  CPoint* head = this->random_point(far_from_wall, far_from_wall);
  CPoint* tail = nullptr;
  std::vector<CPoint*> body;

  DIRECTION const direction = RRND::Core<DIRECTION>::random({ DIRECTION::DOWN, DIRECTION::UP, DIRECTION::LEFT, DIRECTION::RIGHT }); 
  if(direction == DIRECTION::DOWN) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id - static_cast<UI32>(DEFAULT::SNAKE_LENGTH));
    for(UI32 i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); ++i) {
      body.emplace_back(grid_->at(head_id - i));
    }    
  }

  if (direction == DIRECTION::UP) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id + static_cast<UI32>(DEFAULT::SNAKE_LENGTH));
    for (UI32  i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); i++) 		{
      body.emplace_back(grid_->at(head_id + i));
    }
  }

  if (direction == DIRECTION::LEFT) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id + static_cast<UI32>(DEFAULT::SNAKE_LENGTH) * grid_->height());
    for (UI32 i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); i++) 		{
      body.emplace_back(grid_->at(head_id + grid_->height() * i));
    }
  }

  if (direction == DIRECTION::RIGHT) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id - grid_->height() * static_cast<UI32>(DEFAULT::SNAKE_LENGTH));
    
    for (UI32 i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); i++) 		{
      body.emplace_back(grid_->at(head_id - grid_->height() * i));
    }
  }

  for(auto & p : body) {
    p->set_type(POINT_TYPE::SNAKE_BODY);
  }

  head->set_type(POINT_TYPE::SNAKE_HEAD);
  tail->set_type(POINT_TYPE::SNAKE_TAIL);
  CSnake* snake = new CSnake{grid_, head, tail, body, static_cast<UI32>(DEFAULT::SNAKE_LENGTH), direction };
  initialized_direction_ = direction;
  return snake;
}

CFood* CStage::init_food() {
  auto is_boundary = [&](UI32 d) -> bool {
    return ((d != 0) && (d != grid_->width() - 1) && (d != grid_->height() - 1));
  };
  return new CFood{ this->random_point(is_boundary, is_boundary) };
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
