#include <sstream>
#include <iostream>

#include "Point.h"
#include "raylib.h"
#include "ratio_random.h"

UI32 CPoint::size_ = (UI32)DEFAULT::POINT_SIZE;
std::unordered_map<POINT_TYPE, Texture2D> CPoint::point_image_{};

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
  return !(type_ == POINT_TYPE::SNAKE_TAIL    ||
           type_ == POINT_TYPE::SNAKE_HEAD    ||
           type_ == POINT_TYPE::SNAKE_BODY    ||
           type_ == POINT_TYPE::SNAKE_TAIL_PC ||
           type_ == POINT_TYPE::SNAKE_HEAD_PC ||
           type_ == POINT_TYPE::SNAKE_BODY_PC ||
           type_ == POINT_TYPE::FOOD);
}

void CPoint::set_size(UI32 t_size) {
    size_ = t_size;
}

UI32 CPoint::get_size() {
  return size_;
}
// TODO: adjust texture with point size
void CPoint::initialize_point_image() {
  std::unordered_map<POINT_TYPE, char const *> image_name{
    { POINT_TYPE::SPACE           , "resources/steel.png"       },
    { POINT_TYPE::OBSTACLE        , "resources/brick.png"       },
    { POINT_TYPE::PATH_FOUND_PATH , "resources/pink_tile.png"   },
    { POINT_TYPE::FOOD            , "resources/watermellon.png" },
    { POINT_TYPE::SNAKE_BODY      , "resources/snake_skin.png"  },
    { POINT_TYPE::SNAKE_TAIL      , "resources/snake_skin.png"  }
  };
  
  for(auto& p : image_name) {
    Texture2D texture = LoadTexture(p.second);
    texture.height = CPoint::size_;
    texture.width = CPoint::size_;
    point_image_.insert(std::make_pair(p.first, texture));
  }
}

void CPoint::set_type(POINT_TYPE t_type) {
  this->type_ = t_type;
}

void CPoint::draw() const {
  float const GAP_SIZE = 0.2f;
  Rectangle r{ this->x_ * CPoint::size_, this->y_ * CPoint::size_, CPoint::size_ - GAP_SIZE, CPoint::size_ - GAP_SIZE };

  if (point_image_.find(type()) == point_image_.end()) 	{
    DrawRectangle(r.x, r.y, r.width, r.height, G_TYPE_COLOR.at(this->type_));
  } else {
    DrawTexture(point_image_.at(type()), r.x, r.y, WHITE);
  }

  // for checking heuristic value; require draw full
  //if (type() != POINT_TYPE::SPACE) 	{
  //  UI32 pos_x = x_ * CPoint::size_;
  //  UI32 pos_y = y_ * CPoint::size_;
  //  std::stringstream ss;
  //  ss << heuristic_value_;
  //  DrawText(ss.str().c_str(), pos_x, pos_y, 5, RED);
  //}

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
