#include "server.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS true
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

Server::Server(int port) {
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(port);
}


bool Server::InitWSA() {


	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return false;
	}



	return true;
}

bool Server::CleanupWSA() {
	WSACleanup();
	return true;
}

bool Server::InitSocket() {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		wprintf(L"Error empty %d \n", WSAGetLastError());
		WSACleanup();
		return false;
	}

	return true;
}

bool Server::BindSocket() {

	iResult = bind(sock, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return false;
	}

	return true;
}

bool Server::ListenConnect() {
	iResult = listen(sock, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return false;
	}

	return true;

}


bool Server::AcceptConnect() {
	if (sock == INVALID_SOCKET) {
		return false;
	}
	cs = accept(sock, NULL, NULL);
	if (!running) return false;
	if (cs == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		return false;
	}
	
	return true;
}

void getRequestHeaders(std::string requestData) {
	std::string result;

	std::istringstream iss(requestData);

	for (std::string line; std::getline(iss, line); )
	
	
	{
		std::string delimiter = ":";
		if (line.find(delimiter, delimiter.size())) {
			std::string name = line.substr(0, line.find(":"));
			std::cout << line << "\n";
			std::string value = line.substr(line.find(":")+1);
			std:: cout << value << "\n";



		}
	}
}

bool Server::ConnectLoop() {
 do {
	 if (cs == INVALID_SOCKET) {
		 printf("Error: Trying to receive on an invalid socket! Connection was closed before!\n");
		 return false;
	 }
	 received = recv(cs, receiveBuffer, receiveBufferLength, 0);
    if (received> 0) {
      printf("Bytes received: %d\n", 151);

      std::string request(receiveBuffer);
      std::cout << request;
	  getRequestHeaders(request);
      iSendResult = send(cs, headers, strlen(headers), 0);
      if (iSendResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(cs);
		cs = INVALID_SOCKET;
        return false;
      }
      printf("Bytes sent: %d\n", iSendResult);
    } else if (received== 0)
      printf("Connection closing...\n");
    else {
      printf("recv failed with error: %d\n", WSAGetLastError());
      closesocket(cs);
	  cs = INVALID_SOCKET;
      return false;
    }
	shutdown(cs, SD_BOTH);
	closesocket(cs);
	cs = INVALID_SOCKET;
  } while (received> 0);

  return true;
}

bool Server::CloseSocket() {
	running = false;
	if (sock != INVALID_SOCKET) {
		shutdown(sock, SD_BOTH);
		iResult = closesocket(sock);
		if (iResult == SOCKET_ERROR) {
			printf("closesocket failed with error = %d\n", WSAGetLastError());
		}
		sock = INVALID_SOCKET;
	}

	WSACleanup();
	return true;
}

Server::~Server() {
	if (CloseSocket()) {
		wprintf(L"Server closed");
	}
}