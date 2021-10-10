#include <sstream>
#include <iostream>

#include "Point.h"
#include "raylib.h"
#include "ratio_random.h"

UI32 CPoint::size_ = (UI32)DEFAULT::POINT_SIZE;

CPoint::CPoint(UI32 const t_x, UI32 const t_y, POINT_TYPE const t_type)
  : x_(t_x),
  y_(t_y),
  type_(t_type) {
}

UI32 CPoint::x() const {
  return x_;
}

UI32 CPoint::y() const {
  return y_;
}

POINT_TYPE CPoint::type() const {
  return type_;
}

bool CPoint::is_reachable() const {
  return ( type() != POINT_TYPE::WALL       && 
           type() != POINT_TYPE::OBSTACLE   &&
           type() != POINT_TYPE::SNAKE_HEAD &&
           type() != POINT_TYPE::SNAKE_TAIL &&
           type() != POINT_TYPE::SNAKE_BODY);
}

bool CPoint::is_modifiable_type() const {
  return !(type_ == POINT_TYPE::SNAKE_TAIL ||
           type_ == POINT_TYPE::SNAKE_HEAD ||
           type_ == POINT_TYPE::SNAKE_BODY ||
           type_ == POINT_TYPE::FOOD);
}

void CPoint::set_size(UI32 t_size) {
    size_ = t_size;
}

UI32 CPoint::get_size() {
  return size_;
}

void CPoint::set_type(POINT_TYPE t_type) {
  this->type_ = t_type;
}

void CPoint::draw() const {
  float const GAP_SIZE = 0.2f;
  Rectangle r{ this->x_ * CPoint::size_, this->y_ * CPoint::size_, CPoint::size_ - GAP_SIZE, CPoint::size_ - GAP_SIZE };
  DrawRectangle(r.x, r.y, r.width, r.height, G_TYPE_COLOR.at(this->type_));

  //UI32 pos_x = x_ * CPoint::size_;
  //UI32 pos_y = y_ * CPoint::size_;
  //std::stringstream ss;
  //ss << x_ << "," << y_;
  //DrawText(ss.str().c_str() , pos_x, pos_y, 5, YELLOW);

  //ss.str("");
  //ss << (x_ * 600/30 + y_);
  //DrawText(ss.str().c_str(), pos_x + 7, pos_y + 7, 5, YELLOW);
}

void CPoint::dump() const {
  std::cout << "(" << x_ << ", " << y_ << ") - " << G_TYPE_NAME.at(type_) << '\n';
}

std::string CPoint::to_string() const {
  std::stringstream ss;
  ss << "(" << x_ << ", " << y_ << ") - " << G_TYPE_NAME.at(type_);
  return ss.str();
}
