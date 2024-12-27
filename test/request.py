import sys
import random
import requests

if len(sys.argv) < 2:
    PORT = input("Port: ") or 8080
else:
    PORT = sys.argv[1]

TEST_TICKER = 'MSFT'
test_ct = 100_000

prices = [int(random.random() * 100)/100 + 300 for i in range(test_ct)]

# generate list of random ints
quantities = [random.randint(1, 10000) for i in range(test_ct)]


def check():
    s = requests.Session()
    buy_order = [
        f'http://localhost:{PORT}/buy_order?ticker={TEST_TICKER}&price={price}'
        f'&quantity={quantity}'
        for price, quantity in zip(prices, quantities)
    ]

    for buy in buy_order:
        s.get(buy)

    sell_order = [
        f'http://localhost:{PORT}/sell_order?ticker={TEST_TICKER}'
        f'&price={price}&quantity={quantity}'
        for price, quantity in zip(prices, quantities)
    ]

    for sell in sell_order:
        s.get(sell)

    res = s.get(f'http://localhost:{PORT}/all_orders?'
                f'ticker={TEST_TICKER}').json()
    for x in res:
        for y in res[x]:
            for z in res[x][y]:
                if z != []:
                    raise Exception


check()
