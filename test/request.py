import time
import random
import requests
import subprocess

# subprocess.Popen(["make", "run"])

time.sleep(1)

PORT = input("Port: ") or 8080
TEST_TICKER = 'MSFT'
prices = [int(random.random() * 100)/100 + 300 for i in range(100)]
quantities = [random.randint(1, 10000) for i in range(1000)]  # generate list of random ints

buy_order = [
    f'http://localhost:{PORT}/buy_order?ticker={TEST_TICKER}&price={price}&quantity={quantity}'
    for price, quantity in zip(prices, quantities)
]

for buy in buy_order:
    requests.get(buy)

sell_order = [
    f'http://localhost:{PORT}/sell_order?ticker={TEST_TICKER}&price={price}&quantity={quantity}'
    for price, quantity in zip(prices, quantities)
]

for sell in sell_order:
    requests.get(sell)


res = requests.get(f'http://localhost:{PORT}/all_orders?ticker={TEST_TICKER}').json()

