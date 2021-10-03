#pragma once
#include <unordered_map>
#include <string>

#include "raylib.h"

enum class POINT_TYPE {
  UNDEFINED,
  SPACE,
  FOOD,
  WALL,
  OBSTACLE,
  SNAKE_HEAD,
  SNAKE_BODY,
  SNAKE_TAIL,
  PATH_VISITED,
  PATH_TRAVERSAL,
  PATH_ADJACENT
};

enum class DEFAULT {
  POINT_SIZE = 20,
  SNAKE_LENGTH = 2
};

enum class OBSTACLE_NUM {
  LEVEL_0 =  0
};

enum class LEVEL {
  _0,
};

enum class DIRECTION {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum class SPEED {
  _1 = 1,
  _2,
  _3,
  _4,
  _5,
  _6,
  _7,
  _8,
  _9,
  _10
};

enum class BOUNDARY_STATUS {
  NONE,
  TOP,
  BOTTOM,
  LEFT,
  RIGHT
};


static std::unordered_map<POINT_TYPE, std::string> const G_TYPE_NAME {
  { POINT_TYPE::UNDEFINED  , "Undefined"  },
  { POINT_TYPE::SPACE      , "Space"      },
  { POINT_TYPE::FOOD       , "Food"       },
  { POINT_TYPE::WALL       , "Wall"       },
  { POINT_TYPE::OBSTACLE   , "Obstacle"   },
  { POINT_TYPE::SNAKE_HEAD , "Snake_head" },
  { POINT_TYPE::SNAKE_BODY , "Snake_body" },
  { POINT_TYPE::SNAKE_TAIL , "Snake_tail" }
};

static std::unordered_map<POINT_TYPE, char> const G_TYPE_CHAR {
  { POINT_TYPE::UNDEFINED  , '?' },
  { POINT_TYPE::SPACE      , ' ' },
  { POINT_TYPE::FOOD       , 'f' },
  { POINT_TYPE::WALL       , 'w' },
  { POINT_TYPE::OBSTACLE   , 'o' },
  { POINT_TYPE::SNAKE_HEAD , 'h' },
  { POINT_TYPE::SNAKE_BODY , 'b' },
  { POINT_TYPE::SNAKE_TAIL , 't' }
};

static std::unordered_map<POINT_TYPE, Color> G_TYPE_COLOR{
  { POINT_TYPE::UNDEFINED      , BLACK     },
  { POINT_TYPE::SPACE          , WHITE     },
  { POINT_TYPE::FOOD           , ORANGE    },
  { POINT_TYPE::WALL           , RED       },
  { POINT_TYPE::OBSTACLE       , DARKBROWN },
  { POINT_TYPE::SNAKE_HEAD     , DARKBLUE  },
  { POINT_TYPE::SNAKE_BODY     , BLUE      },
  { POINT_TYPE::SNAKE_TAIL     , SKYBLUE   },
  { POINT_TYPE::PATH_ADJACENT  , VIOLET    },
  { POINT_TYPE::PATH_TRAVERSAL , PURPLE    },
  { POINT_TYPE::PATH_VISITED   , BLACK     }
};

#define watch(x) std::cout << (#x) << ": " << (x) << '\n'