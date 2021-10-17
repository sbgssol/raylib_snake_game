#include <iostream>

#include "Snake.h"


void CSnake::move(DIRECTION t_direction, bool t_just_eat_food) {
  // TODO [DONE]: Boundary checking
  // BUG: Boundary BOTTOM/RIGHT, direction=RIGHT => DONE
  std::unordered_map<DIRECTION, DIRECTION> direction_checker {
    { DIRECTION::UP    , DIRECTION::DOWN},
    { DIRECTION::DOWN  , DIRECTION::UP},
    { DIRECTION::LEFT  , DIRECTION::RIGHT},
    { DIRECTION::RIGHT , DIRECTION::LEFT}
  };

  if (direction_checker[t_direction] == this->direction_) {
    t_direction = this->direction_;
  }
  
  std::unordered_map<DIRECTION, UI32> new_head{
    { DIRECTION::UP    , grid_->get_point_id(head_) - 1},
    { DIRECTION::DOWN  , grid_->get_point_id(head_) + 1},
    { DIRECTION::RIGHT , grid_->get_point_id(head_) + grid_->height()},
    { DIRECTION::LEFT  , grid_->get_point_id(head_) - grid_->height()}
  };

  auto const boundary_status = grid_->get_boundary_status(grid_->get_point_id(head_));
  if (boundary_status.empty() == false) 	{
    if (t_direction == DIRECTION::UP && (boundary_status.find(BOUNDARY_STATUS::TOP) != boundary_status.end())) {
      new_head[t_direction] += grid_->height();
    }
    if (t_direction == DIRECTION::DOWN && (boundary_status.find(BOUNDARY_STATUS::BOTTOM) != boundary_status.end())) {
      new_head[t_direction] -= grid_->height();
    }
    if (t_direction == DIRECTION::RIGHT && (boundary_status.find(BOUNDARY_STATUS::RIGHT) != boundary_status.end())) {
      new_head[t_direction] = head_->y();
    }
    if (t_direction == DIRECTION::LEFT && (boundary_status.find(BOUNDARY_STATUS::LEFT) != boundary_status.end())) {
      new_head[t_direction] = grid_->height() * grid_->width() - grid_->height() + head_->y();
    }
  }

  //std::cout << __FUNCTION__ << ": direction: " << static_cast<UI32>(t_direction) << ", current head id: " << grid_->get_point_id(head_) << '\n';
  set_direction(t_direction);
  if(t_just_eat_food) {
    body_.insert(body_.begin(), this->head());
    ++length_;
  }
  transfer_body();
  set_head(grid_->at(new_head[t_direction]));
}

CPoint* CSnake::head() const {
  return head_;
}

CPoint* CSnake::tail() const {
  return tail_;
}

DIRECTION CSnake::get_direction() const {
  return direction_;
}

void CSnake::draw() const {
  head_->draw();
  tail_->draw();
  for(auto p : body_) {
    p->draw();
  }
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