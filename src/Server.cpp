#include "../includes/Server.hpp"
#include "../includes/Endpoints.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

struct config_res {
    int clientSocket;
    sockaddr_in address;
    int serverFd;
    int addrlen;
};

config_res serverConfigs(int);
int ClientSocketValue(int, void *, void *, int &clientSocket);
void handleRequest(Exchange &exchange, const std::string &request,
                   int clientSocket);

void startServer(int port) {
    auto [clientSocket, address, serverFd, addrlen] = serverConfigs(port);
    Exchange exchange;

    std::vector<std::thread> threadPool;

    while (true) {
        if (ClientSocketValue(serverFd, &address, &addrlen, clientSocket) < 0) {
            std::cerr << "Accept failed\n";
            exit(1);
        }

        threadPool.emplace_back([clientSocket, &exchange]() {
            char buffer[30000] = {0};
            read(clientSocket, buffer, 30000);
            handleRequest(exchange, std::string(buffer), clientSocket);
            close(clientSocket);
        });
    }

    for (auto &thread : threadPool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void handleRequest(Exchange &exchange, const std::string &request,
                   int clientSocket) {
    size_t endpointStart = request.find(' ') + 1;
    size_t endpointEnd = request.find(' ', endpointStart);
    std::string endpoint =
        request.substr(endpointStart, endpointEnd - endpointStart);

    size_t queryStart = endpoint.find('?');
    std::string path = (queryStart != std::string::npos)
                           ? endpoint.substr(0, queryStart)
                           : endpoint;
    std::string query = (queryStart != std::string::npos)
                            ? endpoint.substr(queryStart + 1)
                            : "";
    std::string response;

    // Route the request based on the path
    if (path == "/buy_order") {
        Quantity unfilled = buy_order(exchange, query);
        if (unfilled == -1)
            response = "HTTP/1.1 400\n\nMissing parameters.\n";
        else
            response = "HTTP/1.1 200 OK\n\n{\"unfilled\": " +
                       std::to_string(unfilled) + "}\n";
    } else if (path == "/sell_order") {
        Quantity unfilled = sell_order(exchange, query);
        if (unfilled == -1)
            response = "HTTP/1.1 400\n\nMissing parameters.\n";
        else
            response = "HTTP/1.1 200 OK\n\n{\"unfilled\": " +
                       std::to_string(unfilled) + "}\n";
    } else if (path == "/all_orders") {
        response = all_orders(exchange, query);
    } else if (path == "/all_tickers") {
        response = all_tickers(exchange);
    } else {
        response = "HTTP/1.1 404 Not Found\n\nEndpoint not found.\n";
    }

    // Send the response
    send(clientSocket, response.c_str(), response.length(), 0);
    std::cout << response;
}

config_res serverConfigs(int port) {
    int serverFd;

    // Create socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed\n";
        exit(EXIT_FAILURE);
    }

    int clientSocket;
    sockaddr_in address;
    int addrlen = sizeof(address);

    // Configure the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket to the port
    while (bind(serverFd, (sockaddr *)&address, addrlen) < 0) {
        std::cerr << "Bind failed on port: " << port << "\n";
        address.sin_port = htons(++port);
    }

    // Start listening for connections
    if (listen(serverFd, 3) < 0) {
        std::cerr << "Listen failed\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << "...\n";
    return {clientSocket, address, serverFd, addrlen};
}

int ClientSocketValue(int serverFD, void *address, void *addrlen,
                      int &clientSocket) {
    return (clientSocket =
                accept(serverFD, (sockaddr *)&address, (socklen_t *)&addrlen));
}
