#pragma once
#include <vector>

#include "type_alias.h"
#include "macro.h"
#include "Point.h"
#include "Grid.h"

class CSnake {
public:
  CSnake() = default;
  CSnake(CGrid* t_grid, CPoint* const t_head, CPoint* const t_tail, std::vector<CPoint*> const& t_body, UI32 const t_length,
    DIRECTION const t_direction, SPEED t_speed = SPEED::_2)
    : grid_(t_grid),
      head_(t_head),
      tail_(t_tail),
      body_(t_body),
      length_(t_length),
      direction_(t_direction),
      speed_(t_speed){
  }

  void move(DIRECTION t_direction);
  DIRECTION get_direction() const;
  
protected:
  void set_head(CPoint* t_head);
  void set_tail(CPoint* t_tail);
  void set_body(std::vector<CPoint*> const& t_body);
  void set_direction(DIRECTION t_direction);
  void transfer_body();

private:
  CGrid*               grid_;
  CPoint*              head_;
  CPoint*              tail_;
  std::vector<CPoint*> body_;
  UI32                 length_;
  DIRECTION            direction_;
  SPEED                speed_;
};

