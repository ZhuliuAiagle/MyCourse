#include<iostream>
#include<string>
#include <thread> // C++ 11 线程库
#include"ClientNet.h"

using namespace std;

ClientNet client;

char msgBuffer[4096];
char recvBuffer[4096];
char listenBuffer[4096];
bool isConnect = false;

thread *t2 = NULL;
thread *t = NULL;


// 监听消息队列中的消息
void Polling() {
	client.ClientListenStart(&isConnect);
}

// 监听服务器的返回并不断地加入消息队列

void Polling_2() {
	client.ProcessQueue(listenBuffer, &isConnect);
}


int main()
{
	int rlt = 0;
	string msg;

	t = new thread(Polling);
	t2 = new thread(Polling_2);

	if (rlt == 0)
		// socket连接成功
	{
		//发送消息
		printf("connect successfully. input `q` to quit, `help` for instructions\n");
		cout << endl;
		cout << "\t1. connect\n" << endl;
		cout << "\t2. disconnect\n" << endl;
		cout << "\t3. get server name\n" << endl;
		cout << "\t4. get client list\n" << endl;
		cout << "\t5. get current time\n" << endl;
		cout << "\t6. send info to client\n" << endl;
		cout << endl;
		//同时开始消息队列轮询
		while (1)
		{
			while (!client.real_test_lock());
			printf("localhost/> ");
			getline(cin, msg);
			if (msg == "q") {
				// 如果有活跃连接，先断开连接
				if (isConnect = true) {
					cout << "please wait...";
					isConnect = false;
					Sleep(1000);
					client.ClientClose();
					cout << "done" << endl;
				}
				break;
			}
			else if (msg == "help") {
				cout << endl;
				cout << "\t1. connect\n" << endl;
				cout << "\t2. disconnect\n" << endl;
				cout << "\t3. get server name\n" << endl;
				cout << "\t4. get client list\n" << endl;
				cout << "\t5. get current time\n" << endl;
				cout << "\t6. send info to client\n" << endl;
				cout << "\t`q`to quit, `help`for instructions" << endl;
				cout << endl;
			}
			else
			{
				if (msg == "1") {
					if (isConnect == false) {
						int port;
						string ip;
						cout << "IP: ";
						getline(cin, ip);
						rlt = client.ClientConnect(2459, ip.c_str());
						if (rlt != 0) {
							cout << "Error: Connection failed. ( " << rlt << " )\n" << endl;
							continue;
						}
						cout << "Connection success!\n" << endl;
						isConnect = true;
					}
					else {
						cout << "Error: Can't connect to more than ONE server.\n" << endl;
					}
				}
				else if (msg == "2") {
					string opt;
					cout << "Sure to disconnect? [Y/N]: ";
					getline(cin, opt);
					if (opt == "Y") {
						// 断开连接
						// 首先需要中止两个轮询进程,采用信号变量实现
						if (isConnect == true) {
							cout << "Please waiting...";
							isConnect = false;
							// 然后关闭
							Sleep(1000);
							// 释放线程指针
							cout << "Disconnect success\n" << endl;
							strcpy_s(msgBuffer, "exit");
							// client.ClientSend(msgBuffer, 4096);
							client.ClientClose();
						}
						else {
							cout << "Error: No active connections. Please create one first.\n" << endl;
						}
						
					}
					else if(opt == "N"){
						cout << "Warning: Operation abort.\n" << endl;
					}
					else {
						cout << "Error: No such option.\n" << endl;
					}

				}
				else if (msg == "3") {
					strcpy_s(msgBuffer, "GetComputerName");
					rlt = client.ClientSend(msgBuffer, 4096);
					msgBuffer[0] = '\0';
				}
				else if (msg == "4") {
					strcpy_s(msgBuffer, "GetClientList");
					rlt = client.ClientSend(msgBuffer, 4096);
					msgBuffer[0] = '\0';
				}
				else if (msg == "5") {
					strcpy_s(msgBuffer, "time");
					rlt = client.ClientSend(msgBuffer, 4096);
					msgBuffer[0] = '\0';

				}
				else if (msg == "6") {
					string num;
					string msg;
					cout << "Please select the client No. : ";
					getline(cin, num);
					cout << "Please enter the info: ";
					getline(cin, msg);
					string pkg = "Send " + num + " " + msg;
					strcpy_s(msgBuffer, pkg.c_str());
					rlt = client.ClientSend(msgBuffer, 4096);
					msgBuffer[0] = '\0';
				}
				else {
					cout << "Error: No such option\n" << endl;
				}
			}
		}
		// 关闭socket
	}
	system("pause");
	return 0;
}