#include "../includes/Server.hpp"

int main(int argc, char **argv) {
    if (argc)
        startServer(atoi(argv[0]));
    else
        startServer();
}
