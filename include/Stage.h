#pragma once
#include <functional>
#include <queue>
#include <stack>

#include "Grid.h"
#include "Background.h"
#include "Snake.h"
#include "Food.h"

class CStage {
public:
  CStage() = default;
  CStage(SScreenInfo t_screen_info, LEVEL t_level);
  ~CStage();

  void      move_snake(DIRECTION t_d);
  DIRECTION get_snake_direction() const;
  DIRECTION get_initialized_snake_direction() const;
  bool      is_food_collided() const;
  void      handle_food_collision();
  void      expand_frontier();

  void draw() const;
  void dump() const;
protected:
  void    spawn_food();
  CSnake* init_snake(SNAKE_TYPE t_snake_type = SNAKE_TYPE::USER);
  CFood*  init_food();
  void    init_rest_points();
  CPoint* random_point() const;
  CPoint* random_point(std::function<bool(UI32)> t_x_constraint, std::function<bool(UI32)> t_y_constraint) const;
  void    init_graph();

  void    use_bfs();
  void    use_dijkstra();
  void    use_dfs();
  void    use_a_star();
  void    trace_path();

  void    draw_grid() const;
  void    draw_food() const;
  void    draw_snake() const;
  void    draw_path() const;
  void    draw_obstacles() const;

  std::stack<CPoint*>          dfs_frontier_;
  std::queue<CPoint*>          bfs_frontier_;
  std::list<CPoint*>           dijsktra_frontier_;
  std::unordered_set<CPoint*>  dijkstra_visited;
  std::list<CPoint*>           astar_frontier_;
  std::unordered_set<CPoint*>  astar_visited_;
  std::vector<CPoint*>         shortest_path_;
  
  bool                frontier_initialized_{ false };
  bool                food_found_{ false };

  DIRECTION    initialized_direction_{ DIRECTION::UP };
  bool         just_eat_food_ {false};
private:
  CGrid*       grid_;
  CBackground* background_;
  CFood*       food_;
  CSnake*      snake_;
  CSnake* pc_snake_;
};