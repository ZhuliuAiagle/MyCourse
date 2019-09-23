#pragma once

/*
ClinetNet.h
�ͻ�����
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
	timed_mutex qutex; //����ͬ����
	Queue infoQueue;

public:
	// ����ָ���ķ�����
	int ClientConnect(int port, const char* address);
	// ������Ϣ
	int ClientSend(const char* msg, int len);
	// �ر�����
	void ClientClose();
	// ������Ϣ
	int  ClientReceive(char* recvBuffer);
	// ����������������������
	void ClientListenStart(bool *isConnect);
	// ����������ĺ����ڲ���δ������ʱ�򲻻����Դ���м���
	bool real_test_lock();
	// ������Ϣ���е����̣߳���Ϣ���еĶ����������Դ�ļ���ͷ�ļ���
	void ProcessQueue(char* listenBuffer, bool *isConnect);
};

