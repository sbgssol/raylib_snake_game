#pragma once
#include "type_alias.h"
#include "macro.h"

class CPoint {
public:
  CPoint(UI32 const t_x, UI32 const t_y, POINT_TYPE const t_type)
    : x_(t_x),
      y_(t_y),
      type_(t_type) {
  }

  POINT_TYPE type() const {
    return type_;
  }

  static void set_size(UI32 t_size) {
    size_ = t_size;
  }

  void set_type(POINT_TYPE t_type) {
    type_ = t_type;
  }

  void Draw() const;

private:
  static UI32 size_; // pixel
  UI32        x_;
  UI32        y_;
  POINT_TYPE  type_;
};