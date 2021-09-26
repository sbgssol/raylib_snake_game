#pragma once
#include "type_alias.h"

struct CFrameInfo {
public:
  CFrameInfo(UI32 t_w, UI32 t_h) : width_(t_w), height_(t_h){}

  UI32 width() const {
    return width_;
  }

  UI32 height() const {
    return height_;
  }
private:
  UI32 width_;
  UI32 height_;
};