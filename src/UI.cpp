#include "../includes/UI.hpp"
#include <iomanip>
#include <iostream>
#include <unordered_set>

void main_dialog();
void error_dialog(int response);
void temp_input(); // TODO: Replace with actual list of tickers
void order();
void print();

Exchange NASDAQ, NYSE;
std::unordered_map<std::string, std::unordered_set<std::string>> KNOWN_TICKERS;
Exchanges *EXCHANGES;

void startUI(Exchanges &ex) {
    EXCHANGES = &ex;
    ex["NASDAQ"] = NASDAQ;
    ex["NYSE"] = NYSE;
    short response;
    while (true) {
        main_dialog();
        std::cin >> response;
        if (response == 0)
            break;
        else if (response == 1)
            print();
        else if (response == 2)
            order();
        else
            error_dialog(response);
    }
}

// TODO: Replace with actual list of tickers

void main_dialog() {
    using std::cout;
    cout << "------------------------\n"
         << "|0.                Exit|\n"
         << "|1.        Print Orders|\n"
         << "|2.         Place Order|\n"
         << "------------------------\n"
         << "Choice: ";
}

void error_dialog(int response) {
    using std::cout;
    cout << "----------------------\n"
         << "|Invlaid Input: " << std::setw(5) << std::right << response
         << "|\n"
         << "----------------------\n";
}

// TODO: 'ls' type help command
std::pair<std::string, std::string> exchange_ticker_dialog() {
    using std::cout, std::cin;
    std::string exchange, ticker;
    cout << "Exchange: ";
    cin >> exchange;
    if (KNOWN_TICKERS.find(exchange) == KNOWN_TICKERS.end()) {
        cout << "------------------------------\n"
             << "|Unknown Exchange: " << std::setw(10) << std::right << exchange
             << "|\n"
             << "------------------------------\n";

        cout << "Known Exchanges: ";
        for (const auto TICKER : KNOWN_TICKERS) {
            cout << TICKER.first << ", ";
        }
        cout << "\b\b  \n"; // get rid of final comma
        return {};
    }
    cout << "Ticker: ";
    cin >> ticker;
    if (KNOWN_TICKERS[exchange].find(ticker) == KNOWN_TICKERS[exchange].end()) {
        cout << "|----------------------------|\n"
             << "|Unknown Ticker: " << std::setw(12) << std::right << ticker
             << "|\n|----------------------------|\n";

        cout << "Known Tickers : ";
        for (const auto TICKER : KNOWN_TICKERS[exchange]) {
            cout << TICKER << ", ";
        }
        cout << "\b\b  \n"; // get rid of final comma
        return {};
    }
    return {exchange, ticker};
}

bool valid(std::pair<std::string, std::string> exchange_ticker) {
    return !(exchange_ticker.first == " " || exchange_ticker.second == " ") &&
           EXCHANGES->find(exchange_ticker.first) != EXCHANGES->end();
}

Ledger *getFacet() {
    std::pair<std::string, std::string> res = exchange_ticker_dialog();
    if (!valid(res))
        return nullptr;
    return (*EXCHANGES)[res.first][res.second];
}

void print() {
    auto x = getFacet();
    if (!x)
        return;
}

void order() {
    using std::cout, std::cin;

    auto facet = getFacet();
    if (!facet)
        return;
    float price;
    int volume;
    cout << "Price: $";
    cin >> price;
    cout << "Volume: ";
    cin >> volume;
    cout << "------------------------\n"
         << "|0.           Buy Order|\n"
         << "|1.          Sell Order|\n"
         << "------------------------\n"
         << "Choice: ";
    int response;
    cin >> response;
    std::string line1 =
        (response == 0) ? "---Buy-Order-Summary---" : "--Sell-Order-Summary---";

    cout << ((response == 0) ? "---Buy-Order-Summary---\n"
                             : "--Sell-Order-Summary---\n")
         << "|Price: " << std::setw(6) << std::right << "$" << std::fixed
         << std::setprecision(2) << price << "|\n"
         << "|Volume: " << std::setw(12) << std::right << volume << "|\n"
         << "-----------------------\n";
    facet->place_order(price, volume, !response);
}
