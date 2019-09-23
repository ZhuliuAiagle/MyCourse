#include <winSock2.h>
#include <winsock.h>
#include <iostream>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <time.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define IP_SIZE 129
//设置监听端口为学号后四位 2459
#define ServerPort 2459
//一个包传输的最大容量
#define MaxSize 4096



//Server 类，实现了一系列初始化服务器的操作
class Server
{
public:
	Server();	//初始化服务器操作
	~Server();	//结束服务器操作
	Server(const Server &) = delete;
	Server &operator=(const Server &) = delete;
	void WaitForClient();	//成功监听后等待客户端链接的函数

private:
	WORD winsockVersion;
	WSADATA wsaData;
	SOCKET socketServer;	//服务器socket句柄
	SOCKET socketClient;	//客户端socket句柄
	HANDLE hThread;			//线程
	SOCKADDR_IN addrServer;	//服务器数据
	SOCKADDR_IN addrClient;	//客户端数据
	int retValue; //标记，用来标记各自初始化操作是否成功
	int addrLength;	//数据长度
	//char bufferForIP[IP_SIZE];
	wchar_t bufferForIP[IP_SIZE];
};

//客户端序列用来实现客户端之间的通信
typedef struct client *Clit;
struct client
{
	int Num;			//编号
	SOCKET sockClient;	//socket句柄
	char IP[20];		//IP地址
	u_short Port;		//端口
	Clit Next;			//下一个客户端

};

Clit Head;	//全局变量head,用来存储客户端列表


//往客户端列表里添加新的客户端
int addNode(Clit Head, SOCKET newSock, char* newIP, u_short newPort);
//当一个客户端取消链接后删除客户端
int deleteNode(Clit Head, SOCKET oldSock);
//在客户端列表中获取客户端信息并设置成特定的文件格式放入缓冲区中
int getTheList(Clit Head, char * Message);
//得到对应编码的客户端句柄
SOCKET getSock(Clit Head, int n);
//获得对应socket句柄客户端的编号
int getNum(Clit Head, SOCKET sock);
//创建新的线程
DWORD WINAPI CreateClientThread(LPVOID lpParameter);
//

//主函数
int main()
{
	//初始化服务器
	Server svr;
	//初始化客户端列表
	Head = (Clit)malloc(sizeof(struct client));
	Head->Next = NULL;
	Head->Num = 0;
	//初始化完成开始等待客户端链接

	svr.WaitForClient();
	//服务结束
	system("pause");
	return 0;
}
SOCKET getSock(Clit Head, int n)
{
	Clit ptr = Head->Next;
	SOCKET Find = INVALID_SOCKET;
	while (ptr != NULL)
	{
		if (n == ptr->Num)
		{
			Find = ptr->sockClient;
			break;
		}
		ptr = ptr->Next;
	}
	return Find;
}
int addNode(Clit Head, SOCKET newSock, char* newIP, u_short newPort)
{
	Clit newNode = (Clit)malloc(sizeof(struct client));
	strcpy(newNode->IP, newIP);
	newNode->Port = newPort;
	newNode->Next = NULL;
	newNode->sockClient = newSock;
	newNode->Num = 1;

	Clit ptr = Head;
	while (ptr->Next != NULL)
	{
		newNode->Num++;
		ptr = ptr->Next;
	}
	ptr->Next = newNode;
	cout << "The Client has added to the list" << endl;
	return 0;
}
int deleteNode(Clit Head, SOCKET oldSock)
{
	Clit ptr = Head;
	Clit ptr2 = NULL;
	while (ptr->Next != NULL)
	{
		if (ptr->Next->sockClient == oldSock)
		{
			ptr2 = ptr->Next;
			ptr->Next = ptr2->Next;
			cout << "The Client has been Delete from the list" << endl;
			break;
		}
		ptr = ptr->Next;
	}
	free(ptr2);
	ptr2 = Head;
	int Number = 0;
	while (ptr2 != NULL)
	{
		ptr2->Num = Number;
		ptr2 = ptr2->Next;
		Number++;
	}

	if (ptr->Next == NULL)
	{
		cout << "The Delete()failed" << endl;
	}
	return 1;
}
int getNum(Clit Head, SOCKET sock)
{
	Clit ptr = Head->Next;
	while (ptr != NULL)
	{
		if (ptr->sockClient == sock)
		{
			return ptr->Num;
		}
		ptr = ptr->Next;
	}

	return -1;
}
int getTheList(Clit Head, char * Message)
{
	Clit ptr = Head->Next;
	char Current[10];
	while (ptr != NULL)
	{
		memset(Current, 0, 10);
		sprintf(Current, "%d", ptr->Num);
		strcat(Message, Current);
		strcat(Message, " ");
		strcat(Message, ptr->IP);
		strcat(Message, " ");
		memset(Current, 0, 10);
		sprintf(Current, "%d", (int)ptr->Port);
		strcat(Message, Current);
		strcat(Message, "\n");
		ptr = ptr->Next;

	}
	return 0;
}
void Server::WaitForClient()
{
	while (true)
	{
		//调用Accept函数，若链接到客户端则继续，否则阻塞该程序
		socketClient = accept(socketServer, (SOCKADDR *)&addrClient, &addrLength);
		//连接失败
		if (socketClient == INVALID_SOCKET)
		{
			printf("Accept Failed\n");
			continue;
		}
		printf("Accept Success\n");
		InetNtopW(addrClient.sin_family, &addrClient, bufferForIP, IP_SIZE);
		//输出客户端的IP和监听端口
		cout << "A new client connected! The IP address: " << inet_ntoa(addrClient.sin_addr) << ", port number: " << ::ntohs(addrClient.sin_port) << endl;
		//将客户端加入到客户端链表中
		addNode(Head, socketClient, inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
		//创建子线程
		hThread = CreateThread(NULL, 0, CreateClientThread, (LPVOID)socketClient, 0, NULL);
		if (hThread == NULL)
		{
			cerr << "Failed to create a new thread!Error code: " << ::WSAGetLastError() << endl;
			WSACleanup();
			system("pause");
			exit(1);
		}
		CloseHandle(hThread);
	}
	return;
}
DWORD WINAPI CreateClientThread(LPVOID lpParameter)
{
	//得到客户端Socket句柄
	SOCKET socketClient = (SOCKET)lpParameter;
	//新建缓冲区
	char bufMessage[MaxSize];
	//判断变量，用来判断接收与发送的操作是否完成
	int retValue = 0;
	int sendResult = 0;
	//用来发送给其它客户端的缓冲区
	char AllMessage[MaxSize];
	memset(AllMessage, 0, MaxSize);
	SOCKET Mark = INVALID_SOCKET;

	//连上客户端，发送HELLO
	memset(bufMessage, 0, MaxSize);
	strcpy(bufMessage, "Hello!\n");
	sendResult = send(socketClient, bufMessage, MaxSize, 0);
	do
	{

		memset(bufMessage, 0, MaxSize);
		retValue = recv(socketClient, bufMessage, MaxSize, 0);
		if (retValue > 0)
		{
			if (strcmp(bufMessage, "exit") == 0)
			{
				cout << "Message received: " << bufMessage << endl;
				cout << "Client requests to close the connection" << endl;
				break;
			}
			else if (strstr(bufMessage, "time") != NULL)
			{
				//输出客户端的请求信息
				cout << "Message received: " << bufMessage << endl;
				//重置Buffer
				memset(bufMessage, 0, MaxSize);
				cout << "Client requests to get the time" << endl;
				//得到当前时间并把它复制到Buffer里
				time_t t = time(0);
				strftime(bufMessage, sizeof(bufMessage), "%Y-%m-%d %H-%M-%S\n", localtime(&t));

			}
			else if (strstr(bufMessage, "GetComputerName") != NULL)
			{
				//输出客户端的请求信息并充值buffer
				cout << "Message received: " << bufMessage << endl;
				memset(bufMessage, 0, MaxSize);
				//向主机请求主机名并拷贝入Buffer
				char host[256];
				if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
				{
					cout << "无法获取主机名..." << endl;
					break;
				}
				else
				{
					cout << "The name of the computer: " << host << endl;
					const char* str = "The name of the computer: ";
					strcpy(bufMessage, str);
					strcat(bufMessage, host);
					cout << bufMessage << endl;

				}
			}
			else if (strstr(bufMessage, "GetClientList") != NULL)
			{
				cout << "Message received: " << bufMessage << endl;
				memset(bufMessage, 0, MaxSize);
				getTheList(Head, bufMessage);
				cout << bufMessage << endl;

			}
			else if (strstr(bufMessage, "Send") != NULL)
			{
				//收到send 命令，先打印接收到的消息
				cout << "Message received: " << bufMessage << endl;
				//得到接收客户端的编号并提取 Socket
				int n = bufMessage[5] - '0';
				if (!(1 <= n && n <= 9))
				{
					memset(bufMessage, 0, MaxSize);
					strcpy(bufMessage, "The Number is Error");
					cout << bufMessage << endl;
				}
				else
				{
					Mark = getSock(Head, n);
					if (Mark == INVALID_SOCKET)
					{
						memset(bufMessage, 0, MaxSize);
						strcpy(bufMessage, "The Number is Error,Please check the client list again");
						cout << bufMessage << endl;
					}
					else
					{
						//编辑发送的信息
						char * Message = &(bufMessage[7]);
						memset(AllMessage, 0, MaxSize);
						strcpy(AllMessage, "The Message From Client ");
						char a[5];
						//得到发送客户端的编号
						n = getNum(Head, socketClient);
						a[0] = n + '0';
						a[1] = ' ';
						a[2] = ':';
						a[3] = ' ';
						a[4] = '\0';
						strcat(AllMessage, a);
						//将信息拷贝入ALLMessafe 发送Buffer
						strncat(AllMessage, Message, MaxSize - 28);
						cout << "The Message send: " << AllMessage << endl;
						//发送消息
						sendResult = send(Mark, AllMessage, MaxSize, 0);
						if (sendResult == SOCKET_ERROR)
						{
							memset(bufMessage, 0, MaxSize);
							strcpy(bufMessage, "Your Message send Failed,please Check the Clinet list again");
							cout << bufMessage << endl;
						}
						else
						{	//发送成功，给发送的客户端发一个发送成功的信息
							memset(bufMessage, 0, MaxSize);
							strcpy(bufMessage, "Your Message send success");
							cout << bufMessage << endl;
						}

					}

				}
			}
			else
			{
				memset(bufMessage, 0, MaxSize);
				strcpy(bufMessage, "Sorry, I can't realize the construct");
			}

			sendResult = send(socketClient, bufMessage, MaxSize, 0);
			if (sendResult == SOCKET_ERROR)
			{
				cerr << "Failed to send message to client!Error code: " << ::GetLastError() << endl;
				::closesocket(socketClient);
				system("pause");
				return 1;
			}
			else
			{
				cout << "Send Success" << endl;
			}

		}
		else if (retValue == 0)
		{
			cout << "connection closed..." << endl;
			break;
		}
		else
		{
			cerr << "Failed to receive message from client!Error code: " << ::GetLastError() << endl;
			closesocket(socketClient);
			deleteNode(Head, socketClient);
			system("pause");
			return 1;
		}
	} while (retValue > 0);
	//
	deleteNode(Head, socketClient);
	retValue = shutdown(socketClient, SD_SEND);
	if (retValue == SOCKET_ERROR)
	{
		cerr << "Failed to shutdown the client socket!Error code: " << ::GetLastError() << "\n";
		::closesocket(socketClient);
		return 1;
	}
	return 0;
}
//
Server::Server()
{
	cout << "Initializing Server!" << endl;
	//初始化操作
	winsockVersion = MAKEWORD(2, 2);  //window socket 的版本 2.2
	addrLength = sizeof(SOCKADDR_IN); //SOCKADDR_IN结构体的大小
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);          //htol将主机字节序long型转换为网络字节序
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //绑定本机的IP地址
	addrServer.sin_port = htons(ServerPort);                      //绑定监听端口

	memset(bufferForIP, 0, IP_SIZE);
	//启动WSA
	retValue = WSAStartup(winsockVersion, &wsaData);
	if (retValue != 0)
	{
		cout << "WSAStartup() failed!" << endl;
		system("pause");
		exit(1);
	}
	cout << "WSA started up successfully!" << endl;
	//socket()
	socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (socketServer == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "socket()Failed" << endl;
		system("pause");
		exit(1);
	}
	cout << "Server socket created successfully!" << endl;
	//Bind()
	retValue = bind(socketServer, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
	if (retValue == SOCKET_ERROR)
	{
		printf("bind()Failed! code: %d\n", WSAGetLastError());
		closesocket(socketServer);
		WSACleanup();
		system("pause");
		exit(1);
	}
	cout << "Server socket bound successfully!" << endl;
	//bind完成，开始监听
	retValue = listen(socketServer, 5);
	if (retValue == SOCKET_ERROR)
	{
		printf("listen() faild! code:%d\n", WSAGetLastError());
		closesocket(socketServer); //关闭套接字
		system("pause");
		exit(1);
	}
	cout << "Server socket started to listen...\n";
	printf("Welcome,the Host %s is running!Now Wating for someone comes in!\n", inet_ntoa(addrServer.sin_addr));
}

Server::~Server()
{
	closesocket(socketServer);
	closesocket(socketClient);
	WSACleanup();
	cout << "The Socket is closed" << endl;
}
