#pragma once

#include "product.hpp"
#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

enum class Movement { Receive, Sale, Return, Adjustment };

struct StockMovement {
  sku;
  std::string move
};
class Inventory {};