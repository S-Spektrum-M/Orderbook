#include "../includes/Server.hpp"
#include <unistd.h>

int main(int argc, char **argv) {
    startServer(argc == 2 ? atoi(argv[1]) : 80 + !(geteuid() == 0) * 8000);
}
