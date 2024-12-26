#include "../includes/Ledger.hpp"

Order::Order(Quantity quantity) : quantity(quantity), time(std::time(0)) {}

bool Order::operator<(const Order &compareTo) const {
    return time < compareTo.time;
}

Quantity Ledger::place_order(Price price, Order order, bool side) {
    // 1. Find outstanding orders that can satisfy and remove
    Quantity outstanding = order.quantity;
    Orders &lookup_level = side ? sell[price] : buy[price];
    while (!lookup_level.empty() && outstanding > 0) {
        auto fill = lookup_level.begin()->quantity;
        if (outstanding >= fill) {
            lookup_level.erase(next(lookup_level.begin(), 0));
            outstanding -= fill;
        } else {
            lookup_level.begin()._M_const_cast()->quantity -= outstanding;
            return 0;
        }
    }
    // Submit outstanding volume;
    if (outstanding) {
        (side ? buy[price] : sell[price]).insert({outstanding});
    }
    return outstanding;
}
