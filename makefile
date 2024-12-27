INTERMEDIATES = build/intermediates/Ledger.o build/intermediates/Endpoints.o build/intermediates/Server.o build/intermediates/main.o
CC_OBJ = g++ --std=c++20 -c
CC = g++ --std=c++20


build/intermediates/main.o:
	mkdir -p build/intermediates
	$(CC_OBJ) src/main.cpp -o build/intermediates/main.o -O3

build/intermediates/Ledger.o:
	mkdir -p build/intermediates
	$(CC_OBJ) src/Ledger.cpp -o build/intermediates/Ledger.o -O3

build/intermediates/Endpoints.o:
	mkdir -p build/intermediates
	$(CC_OBJ) src/Endpoints.cpp -o build/intermediates/Endpoints.o -O3

build/intermediates/Server.o:
	mkdir -p build/intermediates
	$(CC_OBJ) src/Server.cpp -o build/intermediates/Server.o -O3

build/bin/orderbook: $(INTERMEDIATES)
	mkdir -p build/bin
	$(CC) $(INTERMEDIATES) -o build/bin/orderbook -O3

run: build/bin/orderbook
	./build/bin/orderbook

run-test:
	python3 test/request.py

clean:
	rm -rf build

clean-intermediates:
	rm -rf build/intermediates

rebuild:
	make clean
	make run -j 32
