#pragma once
#include <vector>

#include "type_alias.h"
#include "macro.h"
#include "Point.h"
#include "Grid.h"

class CSnake {
public:
  CSnake() = default;
  CSnake(CGrid* t_grid) {
    init(t_grid);
  }

  CSnake(CPoint* const t_head, CPoint* const t_tail, std::vector<CPoint*>& const t_body, UI32 const t_length = static_cast<UI32>(DEFAULT::SNAKE_LENGTH))
    : head_(t_head),
      tail_(t_tail),
      body_(t_body),
      length_(t_length) {
  }
protected:
  void init(CGrid* t_grid);
private:
  CPoint* head_;
  CPoint* tail_;
  std::vector<CPoint*> body_;
  UI32 length_;
  DIRECTION direction_;
};

