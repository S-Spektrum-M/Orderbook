#pragma once
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <set>
#include <unordered_map>
#define BUY true
#define SELL false

struct Order;
struct Ledger;
using Price = std::float_t;
using Quantity = std::uint32_t;
using OrderTime = std::time_t;
using Orders = std::multiset<Order>;
using Side = std::unordered_map<Price, Orders>;
using Exchange = std::unordered_map<std::string, Ledger *>;
using Exchanges = std::unordered_map<std::string, Exchange>;

struct Order {
    Quantity quantity;
    OrderTime time;
    Order(Quantity quantity);
    bool operator<(const Order &compareTo) const;
};

struct Ledger {
    Side buy;
    Side sell;
    void place_order(Price price, Order order, bool side);
};
