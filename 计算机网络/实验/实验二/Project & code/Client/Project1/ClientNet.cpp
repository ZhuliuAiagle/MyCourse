
/*
ClientNet.cpp
*/

#include "ClientNet.h"
#include <iostream>

using namespace std;

/*客户端Socket连接*/


int ClientNet::ClientConnect(int port, const char* address)
{
	int rlt = 0;   //connectflag  0-success 1-WSAStartfailed 2-socketfailed 3-connectfailed
	struct timeval timeout = { 3, 0 };
	//设置send和receive的超时
	struct linger so_linger;
	so_linger.l_onoff = TRUE;
	so_linger.l_linger = 30;
	int z = setsockopt(m_sock,SOL_SOCKET,SO_DONTLINGER,(const char*)&so_linger,sizeof(so_linger));
	if (z)
		perror("setsockopt(2)");
	// 记录错误信息
	int iErrMsg;
	// 启动WinSock
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	iErrMsg = WSAStartup(wVersionRequested, &wsaData);
	if (iErrMsg != NO_ERROR)
		// WSAStartup出现了错误
	{
		printf("failed with WSAStartup error: %d\n", iErrMsg);
		rlt = 1;
		return rlt;
	}

	// 创建Socket
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
		// socket出现了错误
	{
		printf("failed with socket error: %d\n", WSAGetLastError());
		rlt = 2;
		return rlt;
	}


	// 目标服务器数据
	SOCKADDR_IN servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(address);

	// sock与目标服务器连接
	iErrMsg = connect(m_sock, (SOCKADDR*)&servaddr, sizeof(servaddr));
	if (iErrMsg != NO_ERROR)
	{
		printf("failed with connect error: %d\n", iErrMsg);
		rlt = 3;
		return rlt;
	}
	// success
	// start listen

	return rlt;

}

/*客户端发送消息*/
int ClientNet::ClientSend(const char* msg, int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	// 指定sock发送消息
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		// 发送失败
	{
		printf("send msg failed with error: %d\n", iErrMsg);
		rlt = 1;
		return rlt;
	}
	else if (iErrMsg == 0)
	{
		rlt = 3;
		printf("connection timeout.\n");
		return rlt;
	}
	//printf("send msg successfully\n");
	iotex.lock();
	return rlt;
}




/*客户端关闭Socket*/
void ClientNet::ClientClose()
{
	closesocket(m_sock);
}


void ClientNet::ClientListenStart(bool *isConnect) {
	while (1) {
		// 安全地控制线程的中止
		string res;
		if (qutex.try_lock() == false) {
			Sleep(500);
			continue;
		}
		if ((res = infoQueue.Dequeue()) == "") { 
			qutex.unlock(); 
			Sleep(500);
			continue; 
		}
		cout << "\nSERVER/> ";
		cout << res.c_str() << endl << endl;
		qutex.unlock();
		if (real_test_lock() == false) {
			iotex.unlock();
		}
		//每500毫秒中读取一次数据，确保receive要比他的频率快
		else
			cout << "\nlocalhost/> ";
		Sleep(500);
		// 使用之后清空
	}
}



void ClientNet::ProcessQueue(char* listenBuffer, bool *isConnect) {
	while (1) {
		// 安全地控制线程的中止
		int iErrMsg = 0;
		iErrMsg = recv(m_sock, listenBuffer, 4096, 0);
		//出错但连接正常，往往是因为超时
		if (iErrMsg < 0 && (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) continue;
		else if (iErrMsg < 0) {
			//未连接
			continue;
		}
		// 连接途中网络突然中断
		else if (iErrMsg == 0) { cout << "error: Network Suddenly Disconnected." << endl; continue; };
		qutex.lock();
		infoQueue.Enqueue(listenBuffer);
		qutex.unlock();
	}
}


bool ClientNet::real_test_lock() {
	if (iotex.try_lock() == false) {
		return false;
	}
	else {
		iotex.unlock();
		return true;
	}

}
Queue::Queue() {
	head = new struct queueList;
	head->next = NULL;
	tail = head;
}
void Queue::Enqueue(char* msg) {
	if (tail == NULL) return;
	tail->next = new struct queueList;
	tail = tail->next;
	strcpy_s(tail->message, msg);
	tail->next = NULL;
}
string Queue::Dequeue() {
	// if (head == NULL) return "";
	if (head == tail) return "";
	QueueNode temp = head->next;
	string ret = temp->message;
	head->next = temp->next;
	if (tail == temp) tail = head;
	delete temp;
	temp = NULL;
	return ret;
}
Queue::~Queue() {
	delete head;
	head = NULL;
}


