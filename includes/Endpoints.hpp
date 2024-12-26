#pragma once
#include "Ledger.hpp"
#include <unordered_map>

// Function to parse query parameters from the endpoint
std::unordered_map<std::string, std::string>
parseQueryParams(const std::string &query);

[[nodiscard("Discarding Unfilled Order Quantity")]]
Quantity buy_order(Exchange &ex, const std::string &query) noexcept;

[[nodiscard("Discarding Unfilled Order Quantity")]]
Quantity sell_order(Exchange &ex, const std::string &query) noexcept;

std::string all_orders(Exchange &ex, const std::string &query);

std::string all_tickers(Exchange &ex);
