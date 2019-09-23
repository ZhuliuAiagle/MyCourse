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
//���ü����˿�Ϊѧ�ź���λ 2459
#define ServerPort 2459
//һ����������������
#define MaxSize 4096



//Server �࣬ʵ����һϵ�г�ʼ���������Ĳ���
class Server
{
public:
	Server();	//��ʼ������������
	~Server();	//��������������
	Server(const Server &) = delete;
	Server &operator=(const Server &) = delete;
	void WaitForClient();	//�ɹ�������ȴ��ͻ������ӵĺ���

private:
	WORD winsockVersion;
	WSADATA wsaData;
	SOCKET socketServer;	//������socket���
	SOCKET socketClient;	//�ͻ���socket���
	HANDLE hThread;			//�߳�
	SOCKADDR_IN addrServer;	//����������
	SOCKADDR_IN addrClient;	//�ͻ�������
	int retValue; //��ǣ�������Ǹ��Գ�ʼ�������Ƿ�ɹ�
	int addrLength;	//���ݳ���
	//char bufferForIP[IP_SIZE];
	wchar_t bufferForIP[IP_SIZE];
};

//�ͻ�����������ʵ�ֿͻ���֮���ͨ��
typedef struct client *Clit;
struct client
{
	int Num;			//���
	SOCKET sockClient;	//socket���
	char IP[20];		//IP��ַ
	u_short Port;		//�˿�
	Clit Next;			//��һ���ͻ���

};

Clit Head;	//ȫ�ֱ���head,�����洢�ͻ����б�


//���ͻ����б�������µĿͻ���
int addNode(Clit Head, SOCKET newSock, char* newIP, u_short newPort);
//��һ���ͻ���ȡ�����Ӻ�ɾ���ͻ���
int deleteNode(Clit Head, SOCKET oldSock);
//�ڿͻ����б��л�ȡ�ͻ�����Ϣ�����ó��ض����ļ���ʽ���뻺������
int getTheList(Clit Head, char * Message);
//�õ���Ӧ����Ŀͻ��˾��
SOCKET getSock(Clit Head, int n);
//��ö�Ӧsocket����ͻ��˵ı��
int getNum(Clit Head, SOCKET sock);
//�����µ��߳�
DWORD WINAPI CreateClientThread(LPVOID lpParameter);
//

//������
int main()
{
	//��ʼ��������
	Server svr;
	//��ʼ���ͻ����б�
	Head = (Clit)malloc(sizeof(struct client));
	Head->Next = NULL;
	Head->Num = 0;
	//��ʼ����ɿ�ʼ�ȴ��ͻ�������

	svr.WaitForClient();
	//�������
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
		//����Accept�����������ӵ��ͻ�������������������ó���
		socketClient = accept(socketServer, (SOCKADDR *)&addrClient, &addrLength);
		//����ʧ��
		if (socketClient == INVALID_SOCKET)
		{
			printf("Accept Failed\n");
			continue;
		}
		printf("Accept Success\n");
		InetNtopW(addrClient.sin_family, &addrClient, bufferForIP, IP_SIZE);
		//����ͻ��˵�IP�ͼ����˿�
		cout << "A new client connected! The IP address: " << inet_ntoa(addrClient.sin_addr) << ", port number: " << ::ntohs(addrClient.sin_port) << endl;
		//���ͻ��˼��뵽�ͻ���������
		addNode(Head, socketClient, inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));
		//�������߳�
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
	//�õ��ͻ���Socket���
	SOCKET socketClient = (SOCKET)lpParameter;
	//�½�������
	char bufMessage[MaxSize];
	//�жϱ����������жϽ����뷢�͵Ĳ����Ƿ����
	int retValue = 0;
	int sendResult = 0;
	//�������͸������ͻ��˵Ļ�����
	char AllMessage[MaxSize];
	memset(AllMessage, 0, MaxSize);
	SOCKET Mark = INVALID_SOCKET;

	//���Ͽͻ��ˣ�����HELLO
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
				//����ͻ��˵�������Ϣ
				cout << "Message received: " << bufMessage << endl;
				//����Buffer
				memset(bufMessage, 0, MaxSize);
				cout << "Client requests to get the time" << endl;
				//�õ���ǰʱ�䲢�������Ƶ�Buffer��
				time_t t = time(0);
				strftime(bufMessage, sizeof(bufMessage), "%Y-%m-%d %H-%M-%S\n", localtime(&t));

			}
			else if (strstr(bufMessage, "GetComputerName") != NULL)
			{
				//����ͻ��˵�������Ϣ����ֵbuffer
				cout << "Message received: " << bufMessage << endl;
				memset(bufMessage, 0, MaxSize);
				//������������������������Buffer
				char host[256];
				if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
				{
					cout << "�޷���ȡ������..." << endl;
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
				//�յ�send ����ȴ�ӡ���յ�����Ϣ
				cout << "Message received: " << bufMessage << endl;
				//�õ����տͻ��˵ı�Ų���ȡ Socket
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
						//�༭���͵���Ϣ
						char * Message = &(bufMessage[7]);
						memset(AllMessage, 0, MaxSize);
						strcpy(AllMessage, "The Message From Client ");
						char a[5];
						//�õ����Ϳͻ��˵ı��
						n = getNum(Head, socketClient);
						a[0] = n + '0';
						a[1] = ' ';
						a[2] = ':';
						a[3] = ' ';
						a[4] = '\0';
						strcat(AllMessage, a);
						//����Ϣ������ALLMessafe ����Buffer
						strncat(AllMessage, Message, MaxSize - 28);
						cout << "The Message send: " << AllMessage << endl;
						//������Ϣ
						sendResult = send(Mark, AllMessage, MaxSize, 0);
						if (sendResult == SOCKET_ERROR)
						{
							memset(bufMessage, 0, MaxSize);
							strcpy(bufMessage, "Your Message send Failed,please Check the Clinet list again");
							cout << bufMessage << endl;
						}
						else
						{	//���ͳɹ��������͵Ŀͻ��˷�һ�����ͳɹ�����Ϣ
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
	//��ʼ������
	winsockVersion = MAKEWORD(2, 2);  //window socket �İ汾 2.2
	addrLength = sizeof(SOCKADDR_IN); //SOCKADDR_IN�ṹ��Ĵ�С
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);          //htol�������ֽ���long��ת��Ϊ�����ֽ���
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //�󶨱�����IP��ַ
	addrServer.sin_port = htons(ServerPort);                      //�󶨼����˿�

	memset(bufferForIP, 0, IP_SIZE);
	//����WSA
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
	//bind��ɣ���ʼ����
	retValue = listen(socketServer, 5);
	if (retValue == SOCKET_ERROR)
	{
		printf("listen() faild! code:%d\n", WSAGetLastError());
		closesocket(socketServer); //�ر��׽���
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
