#include "../includes/Server.hpp"
#include "../includes/Endpoints.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

void startServer() {
    int serverFd, clientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    int p = PORT;
assign_port:
    address.sin_port = htons(p);

    // Bind the socket to the port
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        ++p;
        goto assign_port;
    }

    // Start listening for connections
    if (listen(serverFd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << p << "...\n";
    Exchange exchange;

    // Accept and handle incoming requests
    while (true) {
        if ((clientSocket = accept(serverFd, (struct sockaddr *)&address,
                                   (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Read the HTTP request
        char buffer[30000] = {0};
        read(clientSocket, buffer, 30000);

        // Handle the request
        handleRequest(exchange, (std::string)buffer, clientSocket);

        // Close the connection
        close(clientSocket);
    }
}

void handleRequest(Exchange &exchange, const std::string &request,
                   int clientSocket) {
    // Parse the HTTP request
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
        buy_order(exchange, query);
        response = "HTTP/1.1 200 OK\n\nBuy order placed successfully.\n";
    } else if (path == "/sell_order") {
        sell_order(exchange, query);
        response = "HTTP/1.1 200 OK\n\nSell order placed successfully.\n";
    } else if (path == "/all_orders") {
        response = all_orders(exchange, query);
    } else {
        response = "HTTP/1.1 404 Not Found\n\nEndpoint not found.";
    }

    // Send the response
    send(clientSocket, response.c_str(), response.length(), 0);
    std::cout << response;
}
