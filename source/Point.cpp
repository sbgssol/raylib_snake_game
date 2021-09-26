#include "Point.h"
#include "raylib.h"

#include "random.h"

UI32 CPoint::size_ = (UI32)DEFAULT::POINT_SIZE;

void CPoint::Draw() const {
  
  Rectangle r{ this->x_ * CPoint::size_, this->y_ * CPoint::size_, CPoint::size_ - 0.5, CPoint::size_ - 0.5 };

  DrawRectangle(r.x, r.y, r.width, r.height, G_TYPE_COLOR.at(this->type_));
}
