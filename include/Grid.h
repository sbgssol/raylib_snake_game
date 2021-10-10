#pragma once
#include <unordered_map>
#include <unordered_set>

#include "Point.h"
#include "type_alias.h"

class CGrid {
public:
  CGrid() = default;
  CGrid(UI32 const t_width, UI32 const t_height, UI32 const t_grid_size);

  UI32 width() const;
  UI32 height() const;
  UI32 grid_size() const;
  UI32 maximum_point_id() const;

  void calculate_heuristic_value(CPoint* t_food_position);

  CPoint* at(UI32 t_id);
  CPoint const& at(UI32 t_id) const;
  std::unordered_set<BOUNDARY_STATUS> get_boundary_status(UI32 t_id) const;
  UI32 get_point_id(CPoint* const& t_point) const;

  void draw() const;
  void draw_full() const;
  void dump() const;
protected:
  void init_grid();
  void use_manhattan_distance(CPoint* t_food_position);
  void use_euclidean_distance(CPoint* t_food_position);
private:
  std::unordered_map<UI32, CPoint> grid_;
  UI32 width_;
  UI32 height_;
  UI32 grid_size_;
  //RRND::Core<UI32> rnd_;
};

