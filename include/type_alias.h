#pragma once
#include <stdint.h>

using UI32 = uint32_t;
using SI32 = int32_t;

struct SScreenInfo {
  UI32 const width_;
  UI32 const height_;
  UI32 const grid_size_;
};