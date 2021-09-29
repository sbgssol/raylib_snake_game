#include <sstream>
#include <iostream>

#include "Point.h"
#include "raylib.h"
#include "ratio_random.h"

UI32 CPoint::size_ = (UI32)DEFAULT::POINT_SIZE;

void CPoint::draw() const {  
  Rectangle r{ this->x_ * CPoint::size_, this->y_ * CPoint::size_, CPoint::size_ - 0.5, CPoint::size_ - 0.5 };
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
