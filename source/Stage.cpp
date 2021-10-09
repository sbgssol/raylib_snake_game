#include <iostream>

#include "Stage.h"
#include "ratio_random.h"

CStage::CStage(SScreenInfo t_screen_info, LEVEL t_level) {
  grid_ = new CGrid{ t_screen_info.width_, t_screen_info.height_, t_screen_info.grid_size_ };
  background_ = new CBackground{ grid_, t_level };
  snake_ = init_snake();
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

bool CStage::food_collision() const {
  return snake_->head() == food_->position();
}

void CStage::handle_food_collision() {
  spawn_food();
  just_eat_food_ = true;
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
    if(grid_->at(id)->type() == POINT_TYPE::UNDEFINED) {
      grid_->at(id)->set_type(POINT_TYPE::SPACE);
    }
  }
}

void CStage::draw() const {
  this->grid_->draw();
}

void CStage::dump() const {
  grid_->dump();
}

CSnake* CStage::init_snake() {
  // random head;
  auto far_from_wall = [&](UI32 d) -> bool {
    return ((d > static_cast<UI32>(DEFAULT::SNAKE_LENGTH)) && 
            (d < grid_->width() - 1 - static_cast<UI32>(DEFAULT::SNAKE_LENGTH) && (d < grid_->height() - 1 - static_cast<UI32>(DEFAULT::SNAKE_LENGTH))));
  };
  CPoint* head = this->random_point(far_from_wall, far_from_wall);
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
    p->set_type(POINT_TYPE::SNAKE_BODY);
  }

  head->set_type(POINT_TYPE::SNAKE_HEAD);
  tail->set_type(POINT_TYPE::SNAKE_TAIL);
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
  while(point->type() != POINT_TYPE::UNDEFINED) {
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
    //food_found_ = true;
    return;
  }
  if (dijsktra_frontier_.empty()) {
    for (auto adj : snake_->head()->adjacent_) {
      adj->cost_so_far_ = 1;
      dijsktra_frontier_.push_back(adj);      
    }
    frontier_initialized_ = true;
  }

  CPoint* p = dijsktra_frontier_.front();
  dijsktra_frontier_.erase(dijsktra_frontier_.begin());
  UI32 new_cost = 0;
  
  if (dijkstra_visited.find(p) == dijkstra_visited.end()) 	{
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

void CStage::trace_path() {
  if (food_found_) {
    CPoint* current = food_->position()->parents_;
    while (current) {
      current->set_type(POINT_TYPE::PATH_FOUND_PATH);
      current = current->parents_;
    }
    food_found_ = false;
  }
}

void CStage::expand_frontier() {
  //use_bfs();
  //use_dfs();
  use_dijkstra();
  trace_path();
}
