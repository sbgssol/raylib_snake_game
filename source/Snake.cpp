//#include <functional>

#include "Snake.h"


void CSnake::move(DIRECTION t_direction) {
  // TODO: Boundary checking => DONE
  std::unordered_map<DIRECTION, DIRECTION> direction_checker {
    { DIRECTION::UP    , DIRECTION::DOWN},
    { DIRECTION::DOWN  , DIRECTION::UP},
    { DIRECTION::LEFT  , DIRECTION::RIGHT},
    { DIRECTION::RIGHT , DIRECTION::LEFT}
  };

  
  std::unordered_map<DIRECTION, UI32> new_head{
    { DIRECTION::UP    , grid_->get_point_id(head_) - 1},
    { DIRECTION::DOWN  , grid_->get_point_id(head_) + 1},
    { DIRECTION::RIGHT , grid_->get_point_id(head_) + grid_->height()},
    { DIRECTION::LEFT  , grid_->get_point_id(head_) - grid_->height()}
  };

  auto const boundary_status = grid_->get_boundary_status(grid_->get_point_id(head_));
  if(boundary_status == IS_BOUNDARY::TOP && t_direction == DIRECTION::UP) {
    new_head[t_direction] += grid_->height();
  }
  if(boundary_status == IS_BOUNDARY::DOWN && t_direction == DIRECTION::DOWN) {
    new_head[t_direction] -= grid_->height();
  }
  if(boundary_status == IS_BOUNDARY::RIGHT && t_direction == DIRECTION::RIGHT) {
    new_head[t_direction] = head_->y();
  }
  if (boundary_status == IS_BOUNDARY::LEFT && t_direction == DIRECTION::LEFT) {
    new_head[t_direction] = grid_->height() * grid_->width() - grid_->height() + head_->y();
  }

  if (direction_checker[t_direction] == this->direction_) {
    t_direction = this->direction_;
  }
  set_direction(t_direction);
  transfer_body();
  set_head(grid_->at(new_head[t_direction]));
}

CPoint* CSnake::head() const {
  return head_;
}

DIRECTION CSnake::get_direction() const {
  return direction_;
}

void CSnake::increase_length(UI32 t_len) {
  
}

void CSnake::set_head(CPoint* t_head) {
  this->head_ = t_head;
  this->head_->set_type(POINT_TYPE::SNAKE_HEAD);
}

void CSnake::set_tail(CPoint* t_tail) {
  this->tail_->set_type(POINT_TYPE::SPACE); //TODO: Not always type=SPACE
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

void CSnake::transfer_body() {
  set_tail(body_.back());
  tail_->set_type(POINT_TYPE::SNAKE_TAIL);

  for(UI32 i = static_cast<UI32>(body_.size()) - 1; i > 0; --i) {
    body_[i] = body_[i - 1];
    body_[i]->set_type(POINT_TYPE::SNAKE_BODY);
  }
  body_.front() = head_;
  body_.front()->set_type(POINT_TYPE::SNAKE_BODY);
}