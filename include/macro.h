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
  SNAKE_HEAD_PC,
  SNAKE_TAIL_PC,
  SNAKE_BODY_PC,
  PATH_VISITED,
  PATH_FOUND_PATH,
  PATH_ADJACENT,
  PATH_CURRENT
};

enum class DEFAULT {
  POINT_SIZE = 20,
  SNAKE_LENGTH = 2
};

enum class OBSTACLE_NUM {
  LEVEL_0 =  60
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

enum class SNAKE_TYPE {
  USER,
  PC
};

static std::unordered_map<SNAKE_TYPE, POINT_TYPE> const G_SNAKE_HEAD_TYPE{
  { SNAKE_TYPE::USER, POINT_TYPE::SNAKE_HEAD },
  { SNAKE_TYPE::PC, POINT_TYPE::SNAKE_HEAD_PC }
};
static std::unordered_map<SNAKE_TYPE, POINT_TYPE> const G_SNAKE_BODY_TYPE{
  { SNAKE_TYPE::USER, POINT_TYPE::SNAKE_BODY },
  { SNAKE_TYPE::PC, POINT_TYPE::SNAKE_BODY_PC }
};
static std::unordered_map<SNAKE_TYPE, POINT_TYPE> const G_SNAKE_TAIL_TYPE{
  { SNAKE_TYPE::USER, POINT_TYPE::SNAKE_TAIL },
  { SNAKE_TYPE::PC, POINT_TYPE::SNAKE_TAIL_PC }
};

static std::unordered_map<POINT_TYPE, std::string> const G_TYPE_NAME {
  { POINT_TYPE::UNDEFINED       , "Undefined"       },
  { POINT_TYPE::SPACE           , "Space"           },
  { POINT_TYPE::FOOD            , "Food"            },
  { POINT_TYPE::WALL            , "Wall"            },
  { POINT_TYPE::OBSTACLE        , "Obstacle"        },
  { POINT_TYPE::SNAKE_HEAD      , "Snake_head"      },
  { POINT_TYPE::SNAKE_BODY      , "Snake_body"      },
  { POINT_TYPE::SNAKE_TAIL      , "Snake_tail"      },
  { POINT_TYPE::SNAKE_HEAD_PC   , "snake_head_pc"   },
  { POINT_TYPE::SNAKE_TAIL_PC   , "snake_tail_pc"   },
  { POINT_TYPE::SNAKE_BODY_PC   , "snake_body_pc"   },
  { POINT_TYPE::PATH_VISITED    , "path_visited"    },
  { POINT_TYPE::PATH_FOUND_PATH , "path_found_path" },
  { POINT_TYPE::PATH_ADJACENT   , "path_adjacent"   },
  { POINT_TYPE::PATH_CURRENT    , "path_current"    }  
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
  { POINT_TYPE::SNAKE_HEAD_PC  , DARKGREEN },
  { POINT_TYPE::SNAKE_BODY     , BLUE      },
  { POINT_TYPE::SNAKE_BODY_PC  , GREEN      },
  { POINT_TYPE::SNAKE_TAIL     , SKYBLUE   },
  { POINT_TYPE::SNAKE_TAIL_PC  , GREEN   },
  { POINT_TYPE::PATH_ADJACENT  , VIOLET    },
  { POINT_TYPE::PATH_FOUND_PATH, PURPLE    },
  { POINT_TYPE::PATH_VISITED   , BLACK     },
  { POINT_TYPE::PATH_CURRENT   , RED       }
};

#define watch(x) std::cout << (#x) << ": " << (x) << '\n'