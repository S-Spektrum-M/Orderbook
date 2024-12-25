build/intermediates/main.o:
	mkdir -p build/intermediates
	g++ src/main.cpp -c -o build/intermediates/main.o -O3 -lcurl

build/intermediates/Ledger.o:
	mkdir -p build/intermediates
	g++ src/Ledger.cpp -c -o build/intermediates/Ledger.o -O3

build/intermediates/Endpoints.o:
	mkdir -p build/intermediates
	g++ src/Endpoints.cpp -c -o build/intermediates/Endpoints.o -O3

build/intermediates/Server.o:
	mkdir -p build/intermediates
	g++ src/Server.cpp -c -o build/intermediates/Server.o -O3

build/bin/orderbook: build/intermediates/main.o build/intermediates/Ledger.o build/intermediates/Server.o build/intermediates/Endpoints.o
	mkdir -p build/bin
	g++ build/intermediates/Server.o build/intermediates/Ledger.o build/intermediates/Endpoints.o build/intermediates/main.o -o build/bin/orderbook -O3

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
