#pragma once
#include <iostream>

#include "type_alias.h"
#include "macro.h"

class CPoint {
public:
  // static method
  static void set_size(UI32 t_size);
  static UI32 get_size();
  static void initialize_point_image();

  // constructors
  CPoint(UI32 const t_x, UI32 const t_y, POINT_TYPE const t_type);

  // getter methods
  UI32       x() const;
  UI32       y() const;
  POINT_TYPE type() const;
  bool       is_reachable() const;
  bool       is_modifiable_type() const;

  // setter methods
  void set_type(POINT_TYPE t_type);

  // dump methods
  void draw() const;
  void dump(std::ostream& os = std::cout) const;
  std::string to_string() const;
public:
  static std::unordered_map<POINT_TYPE, Texture2D> point_image_;
  std::vector<CPoint*>    adjacent_;
  CPoint*                 parents_{ nullptr };
  float                   cost_so_far_{ 9999 };
  float                   heuristic_value_{9999};
  bool                    heuristic_adjusted_{ false };
private:
  static UI32             size_; // pixel
  UI32                    x_;
  UI32                    y_;
  POINT_TYPE              type_;
};