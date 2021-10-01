#include <functional>

#include "Snake.h"


void CSnake::move(DIRECTION t_direction) {
  std::unordered_map<DIRECTION, DIRECTION> direction_checker {
    { DIRECTION::UP    , DIRECTION::DOWN},
    { DIRECTION::DOWN  , DIRECTION::UP},
    { DIRECTION::LEFT  , DIRECTION::RIGHT},
    { DIRECTION::RIGHT , DIRECTION::LEFT}
  };

  if(direction_checker[t_direction] == this->direction_) {
    return;
  }

  std::unordered_map<DIRECTION, std::function<void(void)>> mover {
    { DIRECTION::UP    , std::bind(&CSnake::move_up, this) },
    { DIRECTION::DOWN  , std::bind(&CSnake::move_down, this) },
    { DIRECTION::LEFT  , std::bind(&CSnake::move_left, this) },
    { DIRECTION::RIGHT , std::bind(&CSnake::move_right, this) }
  };

  set_direction(t_direction);
  mover[t_direction]();
}

void CSnake::set_head(CPoint* t_head) {
  this->head_->set_type(POINT_TYPE::SPACE);
  this->head_ = t_head;
  this->head_->set_type(POINT_TYPE::SNAKE_HEAD);
}

void CSnake::set_tail(CPoint* t_tail) {
  this->tail_->set_type(POINT_TYPE::SPACE);
  this->tail_ = t_tail;
  this->tail_->set_type(POINT_TYPE::SNAKE_TAIL);
}

void CSnake::set_body(std::vector<CPoint*> const& t_body) {
  for(auto p : body_) {
    p->set_type(POINT_TYPE::SPACE);
  }
  body_.clear();
  for(auto p : t_body) {
    p->set_type(POINT_TYPE::SNAKE_BODY);
    body_.push_back(p);
  }
}

void CSnake::set_direction(DIRECTION t_direction) {
  this->direction_ = t_direction;
}

void CSnake::move_up() {
  auto p = grid_->at((head_->x() * grid_->height() + head_->y()) - 1);
  set_head(p);
}

void CSnake::move_down() {
  auto p = grid_->at((head_->x() * grid_->height() + head_->y()) + 1);
  set_head(p);
}

void CSnake::move_right() {
  auto p = grid_->at((head_->x() * grid_->height() + head_->y()) + grid_->height());
  set_head(p);
}

void CSnake::move_left() {
  auto p = grid_->at((head_->x() * grid_->height() + head_->y()) - grid_->height());
  set_head(p);

}
