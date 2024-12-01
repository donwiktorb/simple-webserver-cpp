#include "server/server.h"


int main() {
    Server server = Server(80);

    if (!server.InitWSA()) {
        return 1;
    }

    if (!server.InitSocket()) {
        return 1;
    }

    if (!server.BindSocket()) {
        return 1;
    }

    if (!server.ListenConnect()) {
        return 1;
    }

    if (!server.AcceptConnect()) {
        return 1;
    }

    if (!server.ConnectLoop()) {
        return 1;
    }

    return 0;
}
