#pragma once
#include <winSock2.h>
#include <winsock.h>
#include <thread>
#include<cstdio>  
#include<cstdlib>  
#include<fstream>
#include<cstring>  
#include<string>
#include<windows.h>
#include<iostream>

enum { SERVER_PORT = 4734, BUFFER_SIZE = 10000, QUEUE_SIZE = 8, MAX_THREAD = 100};

class Message {
public:
	std::string data;
	int id;
	bool* live;
	int clientSocket;
	Message(char* data, bool* life, int cid, int id) :data(data), live(life), clientSocket(cid), id(id) {}
};
class DisconnectMessage:public Message{public:DisconnectMessage(char* dt, bool* lf, int cid, int id) :Message(dt, lf, cid, id){}};

class Server{
public:
		char buffer[BUFFER_SIZE];
		sockaddr_in serverChannel;
		std::thread* t[MAX_THREAD];
		std::thread* closeThread;
		char rootPath[50];
		char name[50];
		bool live[MAX_THREAD];
		int serverSocket; //服务器的socketID
		Server() {
			WORD wVersionRequested;
			WSADATA wsaData;
			int ret;
			//WinSock初始化：  
			wVersionRequested = MAKEWORD(2, 2); //winsock版本
			ret = WSAStartup(wVersionRequested, &wsaData);
			if (ret != 0)
				printf("WSAStartup() failed!\n");
			//确认WinSock DLL支持版本2.2：  
			//否则提示无效的winsock版本
			if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
				WSACleanup();
				printf("Invalid Winsock version!\n");
			}
			memset(live, false, sizeof(live));
			memset(&serverChannel, 0, sizeof(serverChannel));
			serverChannel.sin_family = AF_INET;
			serverChannel.sin_addr.s_addr = htonl(INADDR_ANY);
			serverChannel.sin_port = htons(SERVER_PORT);
		}
		bool RunServer();
};
