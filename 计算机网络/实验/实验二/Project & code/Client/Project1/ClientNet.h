#pragma once

/*
ClinetNet.h
客户网络
*/

#include <stdio.h>
#include <windows.h>
#include <thread>
#include <mutex>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

typedef char Msg[4096];

struct queueList
{
	Msg message;
	struct queueList * next;

};

typedef struct queueList *QueueNode;


class Queue {
	QueueNode head;
	QueueNode tail;
public:
	Queue();
	void Enqueue(char* msg);
	string Dequeue();
	~Queue();
};

class ClientNet
{
private:
	SOCKET m_sock;
	timed_mutex iotex;
	timed_mutex qutex; //队列同步锁
	Queue infoQueue;

public:
	// 连接指定的服务器
	int ClientConnect(int port, const char* address);
	// 发送信息
	int ClientSend(const char* msg, int len);
	// 关闭连接
	void ClientClose();
	// 接收信息
	int  ClientReceive(char* recvBuffer);
	// 监听服务器发过来的请求
	void ClientListenStart(bool *isConnect);
	// 这个测试锁的函数在测试未上锁的时候不会对资源进行加锁
	bool real_test_lock();
	// 处理消息队列的子线程，消息队列的定义和声明在源文件和头文件中
	void ProcessQueue(char* listenBuffer, bool *isConnect);
};

