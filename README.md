# Siddharth Mohanty Freshman Year Final Project

*Email*: [smohanty22@andrew.cmu.edu](mailto://smohanty22@andrew.cmu.edu)
*Student ID*: 616990
*Affiliation*: School of Computer Science
*Graduation Class*: 2028

## Run Command
```bash
make all
make clean-intermediate
make run
```

## Project Description

This program features a mock API designed to simulate trading on the NYSE, with
a unique focus on pre-coalescing orders. By aggregating smaller orders into
larger, more efficient blocks, the program reduces the overhead associated with
handling individual transactions. This pre-coalescing functionality is
particularly beneficial in High-Frequency Trading (HFT) environments, where the
speed and efficiency of trade execution can significantly impact profitability.
By reducing the number of discrete order submissions, the program minimizes
processing delays and enhances the scalability of trading operations.

In HFT, the ability to optimize execution strategies is critical to capitalizing
on fleeting market opportunities. The pre-coalescing mechanism ensures that
traders can execute large volumes of trades without flooding the order book or
causing unnecessary market disruption. This approach reduces transaction costs
by avoiding multiple small-order fees and helps maintain anonymity, a crucial
factor in avoiding adverse price movements caused by signaling large trades.
Simulating these processes in a mock API allows developers to test and refine
their algorithms in a risk-free environment, ensuring readiness for real-world
deployment.

Moreover, the program's design emphasizes minimizing the market impact of
high-frequency trading activities. By consolidating orders before execution,
the program reduces [slippage](https://www.investopedia.com/terms/s/slippage.asp) and helps maintain price stability in the market.
This is particularly important in scenarios where liquidity is limited, and
large orders could otherwise distort prices. With its combination of operational
efficiency and market-sensitivity, this mock API serves as a tool for
developing, testing, and perfecting trading strategies tailored to the demanding
requirements of HFT systems.

### Endpoints

|       Endpoint      |     URL     |          Arguments          |
|---------------------|-------------|-----------------------------|
|Buy Order Submission | `/buy_order`|`ticker`, `price`, `quantity`|
|Sell Order Submission|`/sell_order`|`ticker`, `price`, `quantity`|
|All Unfilled Orders  | `/all_order`|`ticker`                     |


## Project Structure
```
./
./README.md
./src/
./includes/
```

## Attributions

I, Siddharth Mohanty, confirm that this project is original work pursuant to
Carnegie Mellon University's policy and the School of Computer Science's policy
on academic honesty.

### Citations
- [C++ Standard Template Library](https://isocpp.org/)
