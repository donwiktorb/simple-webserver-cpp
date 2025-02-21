#include "server/server.h"

#include <csignal>
bool serverRunning = true;
Server* globalServer = nullptr;

void SignalHandler(int signum) {
    serverRunning = false;
	printf("Received signal %d, Shutting down server\n", signum);
    if (globalServer) {
        globalServer->CloseSocket();
    }

    exit(signum);
};

int main() {
    Server server(80);
    globalServer = &server;

    signal(SIGINT, SignalHandler);

    if (!server.InitWSA()) return 1;
    if (!server.InitSocket()) return 1;
    if (!server.BindSocket()) return 1;
    if (!server.ListenConnect()) return 1;

    while (serverRunning) {
        if (!server.AcceptConnect()) {
            if (!serverRunning) break;
            printf("Failed to accept connection.\n");
            continue;
        }

        if (!server.ConnectLoop()) {
            if (!serverRunning) break;
            printf("Connection error, continuing to listen for new connections.\n");
            continue;
        }
    }

    return 0;
}
