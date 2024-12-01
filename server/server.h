#define _WINSOCK_DEPRECATED_NO_WARNINGS true
#ifndef SERVER
#define SERVER

#ifndef UNICODE
#define UNICODE 1
#endif
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <string>
#include <map>
#include <sstream>

class Server {
public: 
	Server(int port);
	~Server();

	bool InitWSA();
	bool CleanupWSA();

	bool InitSocket();

	bool BindSocket();
	bool ListenConnect();
	bool AcceptConnect();
	bool ConnectLoop();
	bool DestroyConnect();

	bool CloseSocket();



private: 


	std::map<std::string, std::string> responseData;
	int received{ 0 };
	int iResult{ 0 };

	int iSendResult{ 0 };
	SOCKET sock;
	SOCKET cs;
	sockaddr_in service;

  const char *headers = "HTTP/1.1 200 OK\n"
  "Access-Control-Allow-Origin: *\n"
  "Connection: Keep-Alive\n"
  "Content-Type:text/html; charset=utf-8\n"
  "\n"
  "<html><head</head><body>sup</body></html>";

	int receiveBufferLength = 1024;
	char receiveBuffer[1024];
};
#endif // !1
