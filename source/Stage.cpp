#include <iostream>

#include "Stage.h"
#include "ratio_random.h"

CStage::CStage(SScreenInfo t_screen_info, LEVEL t_level) {
  CGrid::initial_type_ = POINT_TYPE::UNDEFINED;
  grid_ = new CGrid{ t_screen_info.width_, t_screen_info.height_, t_screen_info.grid_size_ };
  background_ = new CBackground{ grid_, t_level };
  snake_ = init_snake();
  pc_snake_ = init_snake(SNAKE_TYPE::PC);
  food_ = init_food();
  init_rest_points();
  init_graph();
}

CStage::~CStage() {
  delete grid_;
  delete background_;
  delete food_;
  delete snake_;
}

void CStage::move_snake(DIRECTION t_d) {
  this->snake_->move(t_d, just_eat_food_);
  just_eat_food_ = false;
}

DIRECTION CStage::get_snake_direction() const {
  return snake_->get_direction();
}

DIRECTION CStage::get_initialized_snake_direction() const {
  return initialized_direction_;
}

bool CStage::is_food_collided() const {
  return snake_->head() == food_->position();
}

void CStage::handle_food_collision() {
  spawn_food();
  just_eat_food_ = true;

  // handle path finding initialization
  frontier_initialized_ = false;
  for(auto p : shortest_path_) {
    if(p->is_modifiable_type()) {
      p->set_type(POINT_TYPE::SPACE);
    }
  }
  shortest_path_.clear();
  astar_visited_.clear();
  astar_frontier_.clear();
}

void CStage::spawn_food() {
  delete food_;
  auto is_boundary = [&](UI32 d) -> bool {
    return ((d != 0) && (d != grid_->width() - 1) && (d != grid_->height() - 1));
  };
  CPoint* point = this->random_point(is_boundary, is_boundary);
  while(point->type() != POINT_TYPE::SPACE) {
    point = this->random_point(is_boundary, is_boundary);
  }
  food_ = new CFood{ point };
}

void CStage::init_rest_points() {
  for(UI32 id = 0; id <= grid_->maximum_point_id(); ++id) {
    if(grid_->at(id)->type() == CGrid::initial_type_) {
      grid_->at(id)->set_type(POINT_TYPE::SPACE);
    }
  }
}

void CStage::draw() const {
  //grid_->draw_full();
  draw_grid();
  draw_food();
  draw_snake();
  draw_path();
  draw_obstacles();
}

void CStage::dump() const {
  grid_->dump();
}

CSnake* CStage::init_snake(SNAKE_TYPE t_snake_type) {
  // random head;
  auto far_from_wall = [&](UI32 d) -> bool {
    return ((d > static_cast<UI32>(DEFAULT::SNAKE_LENGTH)) && 
            (d < grid_->width() - 1 - static_cast<UI32>(DEFAULT::SNAKE_LENGTH) && (d < grid_->height() - 1 - static_cast<UI32>(DEFAULT::SNAKE_LENGTH))));
  };
  CPoint* head = this->random_point(far_from_wall, far_from_wall);
  while (head->type() != CGrid::initial_type_) {
    head = this->random_point(far_from_wall, far_from_wall);
  };
  CPoint* tail = nullptr;
  std::vector<CPoint*> body;

  DIRECTION const direction = RRND::Core<DIRECTION>::random({ DIRECTION::DOWN, DIRECTION::UP, DIRECTION::LEFT, DIRECTION::RIGHT }); 
  if(direction == DIRECTION::DOWN) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id - static_cast<UI32>(DEFAULT::SNAKE_LENGTH));
    for(UI32 i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); ++i) {
      body.emplace_back(grid_->at(head_id - i));
    }    
  }

  if (direction == DIRECTION::UP) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id + static_cast<UI32>(DEFAULT::SNAKE_LENGTH));
    for (UI32  i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); i++) 		{
      body.emplace_back(grid_->at(head_id + i));
    }
  }

  if (direction == DIRECTION::LEFT) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id + static_cast<UI32>(DEFAULT::SNAKE_LENGTH) * grid_->height());
    for (UI32 i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); i++) 		{
      body.emplace_back(grid_->at(head_id + grid_->height() * i));
    }
  }

  if (direction == DIRECTION::RIGHT) {
    UI32 head_id = head->x() * grid_->height() + head->y();
    tail = grid_->at(head_id - grid_->height() * static_cast<UI32>(DEFAULT::SNAKE_LENGTH));
    
    for (UI32 i = 0; i < static_cast<UI32>(DEFAULT::SNAKE_LENGTH); i++) 		{
      body.emplace_back(grid_->at(head_id - grid_->height() * i));
    }
  }
    
  for(auto & p : body) {
    p->set_type(G_SNAKE_BODY_TYPE.at(t_snake_type));
  }
  head->set_type(G_SNAKE_HEAD_TYPE.at(t_snake_type));
  tail->set_type(G_SNAKE_TAIL_TYPE.at(t_snake_type));
  
  CSnake* snake = new CSnake{grid_, head, tail, body, static_cast<UI32>(DEFAULT::SNAKE_LENGTH), direction };
  initialized_direction_ = direction;
  return snake;
}

CFood* CStage::init_food() {
  delete food_;
  auto is_boundary = [&](UI32 d) -> bool {
    return ((d != 0) && (d != grid_->width() - 1) && (d != grid_->height() - 1));
  };
  CPoint* point = this->random_point(is_boundary, is_boundary);
  while(point->type() != CGrid::initial_type_) {
    point = this->random_point(is_boundary, is_boundary);
  }

  std::cout << "Food initialized at: " << point->to_string() << '\n';

  return new CFood{ point };
}

CPoint* CStage::random_point() const {
  return grid_->at(RRND::Basic::random(0U, grid_->maximum_point_id()));
}

CPoint* CStage::random_point(std::function<bool(UI32)> t_x_constraint, std::function<bool(UI32)> t_y_constraint) const {
  CPoint* p = this->random_point();
  while(t_x_constraint(p->x()) == false || 
        t_y_constraint(p->y()) == false) {
    p = this->random_point();
  }
  return p;
}

void CStage::init_graph() {
  auto insert_adj = [=](CPoint* source, CPoint* adj) -> void {
    if (adj->is_reachable()) 	{
      source->adjacent_.push_back(adj);
    }
  };
  for(UI32 id = 0; id <= grid_->maximum_point_id(); ++id) {
    CPoint* p = grid_->at(id);    
    auto boundary_status = grid_->get_boundary_status(id);
    // handle normal cases
    if (boundary_status.empty()) {
      insert_adj(p, grid_->at(id - 1));
      insert_adj(p, grid_->at(id + 1));
      insert_adj(p, grid_->at(id - grid_->height()));
      insert_adj(p, grid_->at(id + grid_->height()));
    }
    // handle for boundary cases
    if (boundary_status.empty() == false) {
      // handle for 4 corners
      if (boundary_status.size() == 2) {
        // top left
        if (boundary_status.find(BOUNDARY_STATUS::TOP) != boundary_status.end() &&
            boundary_status.find(BOUNDARY_STATUS::LEFT) != boundary_status.end()) {
          insert_adj(p, grid_->at(id + 1));
          insert_adj(p, grid_->at(id + grid_->height()));
        }
        // top right
        if (boundary_status.find(BOUNDARY_STATUS::TOP) != boundary_status.end() &&
            boundary_status.find(BOUNDARY_STATUS::RIGHT) != boundary_status.end()) {
          insert_adj(p, grid_->at(id + 1));
          insert_adj(p, grid_->at(id - grid_->height()));
        }
        // bottom left
        if (boundary_status.find(BOUNDARY_STATUS::BOTTOM) != boundary_status.end() &&
            boundary_status.find(BOUNDARY_STATUS::LEFT) != boundary_status.end()) {
          insert_adj(p, grid_->at(id - 1));
          insert_adj(p, grid_->at(id + grid_->height()));
        }
        // bottom right
        if (boundary_status.find(BOUNDARY_STATUS::BOTTOM) != boundary_status.end() &&
            boundary_status.find(BOUNDARY_STATUS::RIGHT) != boundary_status.end()) {
          insert_adj(p, grid_->at(id - 1));
          insert_adj(p, grid_->at(id - grid_->height()));
        }
      }
      // handle for edges
      if(boundary_status.size() == 1) {
        // top
        if(boundary_status.find(BOUNDARY_STATUS::TOP) != boundary_status.end()) {
          insert_adj(p, grid_->at(id - grid_->height()));
          insert_adj(p, grid_->at(id + 1));
          insert_adj(p, grid_->at(id + grid_->height()));
        }
        // bottom
        if (boundary_status.find(BOUNDARY_STATUS::BOTTOM) != boundary_status.end()) {
          insert_adj(p, grid_->at(id - grid_->height()));
          insert_adj(p, grid_->at(id - 1));
          insert_adj(p, grid_->at(id + grid_->height()));
        }
        // left
        if (boundary_status.find(BOUNDARY_STATUS::LEFT) != boundary_status.end()) {
          insert_adj(p, grid_->at(id - 1));
          insert_adj(p, grid_->at(id + grid_->height()));
          insert_adj(p, grid_->at(id + 1));
        }
        // right
        if (boundary_status.find(BOUNDARY_STATUS::RIGHT) != boundary_status.end()) {
          insert_adj(p, grid_->at(id - 1));
          insert_adj(p, grid_->at(id - grid_->height()));
          insert_adj(p, grid_->at(id + 1));
        }
      }
    }
  }
}

void CStage::use_bfs() {
  if (frontier_initialized_ && bfs_frontier_.empty()) {
    return;
  }
  if (bfs_frontier_.empty()) {
    for (auto adj : snake_->head()->adjacent_) {
      if (adj->is_reachable()) {
        bfs_frontier_.push(adj);
      }
    }
    frontier_initialized_ = true;
  }

  bfs_frontier_.front()->set_type(POINT_TYPE::PATH_VISITED);
  CPoint* p = bfs_frontier_.front();
  bfs_frontier_.pop();
  for (auto adj : p->adjacent_) {
    if (adj->is_reachable()) {
      adj->parents_ = p;
      if (adj == food_->position()) {
        food_found_ = true;
        decltype(bfs_frontier_) q;
        std::swap(bfs_frontier_, q);
        break;
      }
      adj->set_type(POINT_TYPE::PATH_ADJACENT);
      bfs_frontier_.push(adj);
    }
  }
}

void CStage::use_dijkstra() {
  if (frontier_initialized_ && dijsktra_frontier_.empty()) {
    //shortest_path_.clear();
    return;
  }
  if (dijsktra_frontier_.empty()) {
    for (auto adj : snake_->head()->adjacent_) {
      adj->cost_so_far_ = 1;
      dijsktra_frontier_.push_back(adj);      
    }
    frontier_initialized_ = true;
  }

  dijsktra_frontier_.sort([](CPoint* p1, CPoint* p2) {
    return p1->cost_so_far_ < p2->cost_so_far_;
  });

  CPoint* p = dijsktra_frontier_.front();
  dijsktra_frontier_.erase(dijsktra_frontier_.begin());
  
  if (dijkstra_visited.find(p) == dijkstra_visited.end()) {
    UI32 new_cost = 0;
    for (auto adj : p->adjacent_) {
      new_cost = p->cost_so_far_ + 1 /*1 == cost from point to point*/;
      if (dijkstra_visited.find(adj) == dijkstra_visited.end() || (new_cost < adj->cost_so_far_)) {
        adj->cost_so_far_ = new_cost;
        adj->parents_ = p;
        if (adj == food_->position()) {
          food_found_ = true;
          decltype(dijsktra_frontier_) q;
          std::swap(dijsktra_frontier_, q);
          break;
        }
        if (adj->is_modifiable_type()) {
          adj->set_type(POINT_TYPE::PATH_ADJACENT);
        }
        dijsktra_frontier_.push_back(adj);
      }
    }
  }

  dijkstra_visited.insert(p);
  if (p->is_modifiable_type()) {
    p->set_type(POINT_TYPE::PATH_VISITED);
  }
}

void CStage::use_dfs() {
  if (frontier_initialized_ && dfs_frontier_.empty()) {
    return;
  }
  if (dfs_frontier_.empty()) {
    for (auto adj : snake_->head()->adjacent_) {
      if (adj->is_reachable()) {
        dfs_frontier_.push(adj);
      }
    }
    frontier_initialized_ = true;
  }
  
  dfs_frontier_.top()->set_type(POINT_TYPE::PATH_VISITED);
  CPoint* p = dfs_frontier_.top();
  dfs_frontier_.pop();
  for (auto adj : p->adjacent_) {
    if (adj->type() == POINT_TYPE::SPACE ||
        adj->type() == POINT_TYPE::FOOD) {
      adj->parents_ = p;
      if (adj == food_->position()) {
        food_found_ = true;
        decltype(dfs_frontier_) q;
        std::swap(dfs_frontier_, q);
        break;
      }
      adj->set_type(POINT_TYPE::PATH_ADJACENT);
      dfs_frontier_.push(adj);
    }
  }
}

void CStage::use_a_star() {
  if (frontier_initialized_ && astar_frontier_.empty()) {
    return;
  }
  if (astar_frontier_.empty()) {
    // initialize heuristic value
    grid_->calculate_heuristic_value(food_->position());

    for (auto adj : snake_->head()->adjacent_) {
      adj->cost_so_far_ = 1;
      astar_frontier_.push_back(adj);
    }
    frontier_initialized_ = true;
  }

  astar_frontier_.sort([](CPoint* p1, CPoint* p2) {
    return (p1->cost_so_far_ + p1->heuristic_value_) < (p2->cost_so_far_ + p2->heuristic_value_);
  });

  CPoint* p = astar_frontier_.front();
  astar_frontier_.pop_front();

  std::sort(p->adjacent_.begin(), p->adjacent_.end(), [](CPoint* p1, CPoint* p2) {
    return (p1->cost_so_far_ + p1->heuristic_value_) < (p2->cost_so_far_ + p2->heuristic_value_);
  }); // to select the lowest cost adjacent

  if (astar_visited_.find(p) == astar_visited_.end()) {
    float new_cost = 0;
    for (auto adj : p->adjacent_) {
      new_cost = p->cost_so_far_ + 1 + adj->heuristic_value_ /*1 == cost from point to point*/;
      if (astar_visited_.find(adj) == astar_visited_.end() || (new_cost < adj->cost_so_far_ + adj->heuristic_value_)) {
        adj->cost_so_far_ = new_cost;
        adj->parents_ = p;
        if (adj == food_->position()) {
          food_found_ = true;
          decltype(astar_frontier_) q;
          std::swap(astar_frontier_, q);
          break;
        }
        if (adj->is_modifiable_type()) {
          adj->set_type(POINT_TYPE::PATH_ADJACENT);
        }
        astar_frontier_.push_back(adj);
        break; // just collect the lowest cost one
      }
    }
  }

  astar_visited_.insert(p);

  if (p->is_modifiable_type()) {
    p->set_type(POINT_TYPE::PATH_VISITED);
  }
}

void CStage::trace_path() {
  if (food_found_) {
    CPoint* current = food_->position()->parents_;
    while (current) {
      current->set_type(POINT_TYPE::PATH_FOUND_PATH);
      shortest_path_.push_back(current);
      current = current->parents_;
    }
    food_found_ = false;
  }
}

void CStage::draw_grid() const {
  grid_->draw();
}

void CStage::draw_food() const {
  food_->position()->draw();
}

void CStage::draw_snake() const {
  snake_->draw();
  pc_snake_->draw();
}

void CStage::draw_path() const {
  if (shortest_path_.empty() == false) {
    for (auto p : shortest_path_) {
      p->draw();
    }
  }
}

void CStage::draw_obstacles() const {
  background_->draw();
}

void CStage::expand_frontier() {
  //use_bfs();
  //use_dfs();
  //use_dijkstra();
  use_a_star();
  trace_path();
}
