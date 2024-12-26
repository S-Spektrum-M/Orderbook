#include "../includes/Endpoints.hpp"
#include <sstream>

std::unordered_map<std::string, std::string>
parseQueryParams(const std::string &query) {
    std::unordered_map<std::string, std::string> params;
    std::stringstream ss(query);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            params[key] = value;
        }
    }
    return params;
}

Quantity buy_order(Exchange &ex, const std::string &query) noexcept {
    auto params = parseQueryParams(query);

    // Validate query parameters
    if (params.find("ticker") == params.end() ||
        params.find("price") == params.end() ||
        params.find("quantity") == params.end()) {
        std::cerr << "Error: Missing required query parameters.\n";
        return -1;
    }

    std::string ticker = params["ticker"];
    Price price = std::stod(params["price"]);
    Quantity quantity = std::stoi(params["quantity"]);
    if (ex.find(ticker) == ex.end()) {
        ex[ticker] = std::make_shared<Ledger>();
        // ex[ticker] = new Ledger();
    }
    return ex[ticker]->place_order(price, quantity, BUY);
}

Quantity sell_order(Exchange &ex, const std::string &query) noexcept{
    auto params = parseQueryParams(query);

    // Validate query parameters
    if (params.find("ticker") == params.end() ||
        params.find("price") == params.end() ||
        params.find("quantity") == params.end()) {
        std::cerr << "Error: Missing required query parameters.\n";
        return -1;
    }

    std::string ticker = params["ticker"];
    Price price = std::stod(params["price"]);
    Quantity quantity = std::stoi(params["quantity"]);
    if (ex.find(ticker) == ex.end()) {
        ex[ticker] = std::make_shared<Ledger>();
        // ex[ticker] = new Ledger();
    }
    return ex[ticker]->place_order(price, quantity, SELL);
}

std::string set_to_str(std::multiset<Order> orders) {
    if (orders.size() == 0)
        return "[],\n";

    std::string str = "[";
    for (auto order : orders) {
        str += std::to_string(order.quantity) + ", ";
    }
    str.pop_back();
    str.pop_back(); // Remove last comma
    str += "],\n";
    return str;
}

std::string orders_to_str(const Price &price, const Orders &orders) {
    std::ostringstream out;
    out.precision(2);
    out.clear();
    out << std::fixed << price;
    return "\t\t\"" + std::move(out).str() + "\": " + set_to_str(orders);
}

std::string all_orders(Exchange &ex, const std::string &query) {
    auto params = parseQueryParams(query);
    // Handle bad endpoint usage
    if (params.find("ticker") == params.end()) {
        return "HTTP/1.1 400 Bad Request\n\nNo ticker Specified.\n";
    }
    if (ex.find(params["ticker"]) == ex.end())
        return "HTTP/1.1 400 Bad Request\n\nUnkown ticker\n";

    Side &buy = ex[params["ticker"]]->buy;
    Side &sell = ex[params["ticker"]]->sell;
    std::string response = "HTTP/1.1 200 OK\n\n{\n\t\"BUY\": {\n";

    for (const auto &[price, quantities] : buy) {
        response += orders_to_str(price, quantities);
    }
    response =
        response.substr(0, response.length() - 2) + "\n\t},\n\t\"SELL\": {\n";
    for (const auto &[price, quantities] : sell) {
        response += orders_to_str(price, quantities);
    }
    response = response.substr(0, response.length() - 2) + "\n\t}\n}\n";
    return response;
}

std::string all_tickers(Exchange &ex) {
    std::string header = "HTTP/1.1 200 OK\n\n";
    std::string response = "[";
    for (auto [k, v] : ex) {
        response += "\"" + k + "\",";
    }
    // pop last charecter if not empty
    if (response.length() > 1) response.pop_back();
    response += "]\n";
    return header + response;
}
