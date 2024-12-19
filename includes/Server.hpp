#include "Ledger.hpp"

void handleRequest(Exchange &exchange, const std::string &request,
                   int clientSocket);
void startServer(int port);
