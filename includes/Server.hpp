#include "Ledger.hpp"
#define PORT 8080

void handleRequest(Exchange &exchange, const std::string &request,
                   int clientSocket);
void startServer();
