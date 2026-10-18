#pragma once

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>

// Helper to sanitize SKU strings by trimming whitespace
inline std::string trimWhitespace(const std::string &str) {
  auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
    return std::isspace(ch);
  });
  auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
               return std::isspace(ch);
             }).base();
  return (start < end) ? std::string(start, end) : std::string();
}

// The struct containing SKU

struct Variant {
  std::string sku;         // Stock Keeping Unit
  std::string productID;   // Unique ID for the product
  std::string alpha_size;  // Size in terms in X and L
  int num_size;            // Size in terms of numbers
  uint32_t colorCode;      // Unique 32 bit color code
  double unit_price = 0.0; // Price of an unit
  double unit_cost = 0.0;  // Cose of an unit
  int quantity = 0;        // Amount of item present
  int reorderLevel = 0;    // Low-stock boundary threshold
  bool active = true;      // Deactivate instead of deleting history

  Variant() = default;

  // Assining smaller names
  Variant(std::string rawSku, std::string prodId, std::string asz, int nsz,
          uint32_t color, double price, double cost, int qty, int reorder,
          bool isActive = true)
      : sku(trimWhitespace(rawSku)),  // member 'sku' receives trimmed 'rawSku'
        productID(std::move(prodId)), // member 'productID' receives 'prodId'
        alpha_size(std::move(asz)),   // member 'alpha_size' receives 'asz'
        num_size(nsz),                // member 'num_size' receives 'nsz'
        colorCode(color),             // member 'colorCode' receives 'color'
        unit_price(price),            // member 'unit_price' receives 'price'
        unit_cost(cost),              // member 'unit_cost' receives 'cost'
        quantity(qty),                // member 'quantity' receives 'qty'
        reorderLevel(reorder), // member 'reorderLevel' receives 'reorder'
        active(isActive)       // member 'active' receives 'isActive'
  {}
};
