#pragma once
#include <unordered_map>

#include "Point.h"
#include "type_alias.h"

class CGrid {
public:
  CGrid() = default;
  CGrid(UI32 const t_width, UI32 const t_height, UI32 const t_grid_size);

  UI32 width() const;
  UI32 height() const;
  UI32 grid_size() const;  

  CPoint* at(UI32 t_id);
  CPoint const& at(UI32 t_id) const;

  void draw() const;
  void dump() const;
protected:
  void init_grid();
private:
  std::unordered_map<UI32, CPoint> grid_;
  UI32 width_;
  UI32 height_;
  UI32 grid_size_;
  //RRND::Core<UI32> rnd_;
};

