#pragma once
#include <unordered_map>

#include "Point.h"
#include "type_alias.h"
#include "ratio_random.h"

class CGrid {
public:
  CGrid() = default;
  CGrid(UI32 const t_width, UI32 const t_height, UI32 const t_grid_size)
    : width_(t_width / t_grid_size),
      height_(t_height / t_grid_size),
      grid_size_(t_grid_size){
    init_grid();
  }

  UI32 width() const {
    return width_;
  }

  UI32 height() const {
    return height_;
  }

  UI32 grid_size() const {
    return grid_size_;
  }

  void draw() const {
    for(auto const& p : grid_) {
      p.second.draw();
    }
  }

  CPoint* at(UI32 t_id) {
    return &grid_.at(t_id);
  }

  CPoint const & at(UI32 t_id) const {
    return grid_.at(t_id);
  }

  void dump() const;

  //CPoint& random_point();
protected:
  void init_grid();
private:
  std::unordered_map<UI32, CPoint> grid_;
  UI32 width_;
  UI32 height_;
  UI32 grid_size_;
  //RRND::Core<UI32> rnd_;
};

