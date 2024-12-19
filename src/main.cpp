#include "../includes/Server.hpp"

int main(int argc, char **argv) {
    if (argc == 2)
        startServer(atoi(argv[1]));
    else
        startServer(8080);
}
