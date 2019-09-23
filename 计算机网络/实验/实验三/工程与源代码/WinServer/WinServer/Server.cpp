  

#include"Server.h"  

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)





void SendMsg(std::string path, Message msg, int msgType = 1){
	//从本地读取文件流
	std::ifstream in(path, std::ios::binary);
	int sp;
	if (!in) {
		// 未找到路径，则返回HTTP:404报错
		msg.data = "HTTP/1.1 404 Not Found\n";
		printf("404: No such Path or Directory\n");
	}
	else {
		in.seekg(0, std::ios_base::end);
		sp = in.tellg();
		char length[20];
		sprintf(length, "%d", sp);
		msg.data = "HTTP/1.1 200 OK\n";
		if (msgType == 1) // 如果是文本或html信息，messageType会被置为1
			msg.data += "Content-Type: text/html;charset=utf-8\nContent-Length: ";
		else { // 如果是图片信息，messageType会被置为0
			printf("this is an img\n");
			msg.data += "Content-Type: img/jpeg; \nContent-Length: ";
		}
		msg.data += length;
		msg.data += "\n\n";
		int total_size = 0;
		int r = send(msg.clientSocket, msg.data.c_str(), strlen(msg.data.c_str()), 0);
		if (r == SOCKET_ERROR) {
			printf("send failed\n");
			*msg.live = false;
			return;
		}
		else {
			printf("send success\n");
		}
		char buffer[100];
		int s = sp + strlen(msg.data.c_str()) + 1;
		int len = sp;
		total_size = 0;
		in.clear();
		in.seekg(0, std::ios_base::beg);

		while (len > 0) {
			memset(buffer, 0, sizeof(buffer));
			int size = sizeof(buffer) < len ? sizeof(buffer) : len;
			total_size += size;
			len -= size;
			in.read(buffer, size);
			int r = send(msg.clientSocket, buffer, size, 0);

			if (r == SOCKET_ERROR) {
				printf("send failed\n");
				*msg.live = false;
				return;
			}
		}
	}
}

void ProcessMsg(Message msg){
	//在解析head和get语句时候使用的flag变量
	bool isGETMethod = false;
	bool isPostMethod = false;
	//存储字符串段临时变量
	std::string temp = "";
	//解析请求种类
	std::string RESTfulmethod = "";
	// 存储body数据的临时变量
	std::string data = "";
	printf("this is the %d client\ndata = %s\n", msg.id, msg.data.c_str());
	// 如果是空信息，中止这一进程
	if (msg.data == "" || msg.data == "\n") {
		*msg.live = false;
		return;
	}
	// 头部指针
	int i = 0;
	//解析http头部
	while (1) {
		if ((i >= strlen(msg.data.c_str()) || msg.data[i] == '\n') && (msg.data[i + 2] == '\n'))break;
		if (msg.data[i] == ' ') {
			if (isGETMethod) {
				data = temp;
				isGETMethod = false;
				break;
			}
			//如果是GET请求，由于需要进一步解析GET请求的路径，这里直接将isGETMethod置为true
			//等到i移动到路径之前的空格时将包内剩下的内容全部储存到data中，进入下一个代码段
			else if (temp == "GET") {
				RESTfulmethod = temp;
				isGETMethod = true;
			}
			//如果是POST，由于信息在body中，不需要提前结束循环
			else if (temp == "POST") {
				RESTfulmethod = temp;
				isPostMethod = true;
			}
			temp = "";
		}
		else if(msg.data[i] != '\n'){
			temp = temp + msg.data[i];
		}
		++i;
	}
	if (RESTfulmethod == "POST") {

		bool startToEnterUsername = false;
		bool startToEnterPassword = false;
		std::string name = "";
		std::string password = "";
		temp = "";
		for (int j = i + 3; j <= strlen(msg.data.c_str()); j++) {
			if (msg.data[j] == '&' || msg.data[j] == '=' || j == strlen(msg.data.c_str())) {
				std::cout << temp << std::endl;
				//进入用户名字段
				if (startToEnterUsername) {
					if (temp == "3160104734") {
						name = temp;
						password = "4734";
					}
					else
						password = "";
					startToEnterUsername = false;
				}
				//开始进入密码字段
				else if (startToEnterPassword) {
					// 如果密码和用户名匹配
					if (temp == password && temp != "") {
						std::cout << "temp=" << temp << " " << "paw=" << password << std::endl;
						char response[200];
						strcpy(response, "<html><body>Welcome,");
						strcat(response, name.c_str());
						strcat(response, "!</body></html>\n");
						int len = strlen(response);
						char length[20];
						sprintf(length, "%d", len);
						msg.data = "HTTP/1.1 200 OK\n";
						msg.data += "Content-Type: text/html;charset=utf-8\nContent-Length: ";
						msg.data += length;
						msg.data += "\n\n";
						msg.data += response;
						printf("%s\n", msg.data.c_str());
						int r = send(msg.clientSocket, msg.data.c_str(), 10000, 0);

						if (r == SOCKET_ERROR) {
							printf("send failed\n");
							*msg.live = false;
							return;
						}
						printf("send success\n");
						*msg.live = false;
						return;
					}
					// 如果密码和用户名不匹配
					else {
						// 构建响应头
						char response[200];
						strcpy(response, "<html><body>login failed: Wrong user or password</body></html>\n");
						int len = strlen(response);
						char length[20];
						sprintf(length, "%d", len);
						msg.data = "HTTP/1.1 200 OK\n";
						msg.data += "Content-Type: text/html;charset=utf-8\nContent-Length: ";
						msg.data += length;
						msg.data += "\n\n";
						msg.data += response;
						printf("%s\n", msg.data.c_str());
						int ret = send(msg.clientSocket, msg.data.c_str(), 10000, 0);
						if (ret == SOCKET_ERROR) {
							printf("send failed\n");
							*msg.live = false;
							return;
						}
						printf("send success\n");
						*msg.live = false;
						return;
					}
					startToEnterPassword = false;
				}
				else if (temp == "login") 
					startToEnterUsername = true;
				else if (temp == "pass") 
					startToEnterPassword = true;
				if (j == data.size())break;
				temp = "";
			}
			else {
				temp = temp + msg.data[j];
			}
		}
		*msg.live = false;
		return;
	}
	else if (RESTfulmethod == "GET" && data != "") {

		msg.data = "";
		if (data.substr(0, 5) == "/dir/") {
			std::string temp = "";
			std::string str1 = "";
			std::string password;
			std::string name;
			std::string path;
			bool txt_flag = false;
			for (int i = 5; i < data.size(); i++) {
				if (data[i] == '.') {
					isGETMethod = true;
				}
				else if (isGETMethod) {
					temp = temp + data[i];
				}
			}

			if (temp == "") {
				closesocket(msg.clientSocket);
				*msg.live = false;
				return;
			}
			std::cout << "temp=" << temp << "," << std::endl;
			if (temp == "txt") {
				path = "dir/txt/" + data.substr(5);
			}
			else if (temp == "html") {
				std::cout << "yes" << std::endl;
				path = "dir/html/" + data.substr(5);
				std::cout << "path=" << path << std::endl;
			}
			std::cout << "temp=" << temp << std::endl;
			SendMsg(path, msg);
		}
		else if (data.substr(0, 5) == "/img/") {
			int total_size;
			int s;
			std::string path = "dir/img/" + data.substr(5);
			SendMsg(path, msg, 0); // 表示这是一个图片
		}
	}
	closesocket(msg.clientSocket);
	*msg.live = false;
	return;
}

void CloseServerThread(DisconnectMessage msg){
	std::string temp;
	while (1) {
		std::cin >> temp;
		if (temp == "quit") {
			while (1) {
				bool flag = true;
				// 将当前活跃的信息全部置为不活跃状态
				for (int i = 0; i < MAX_THREAD; i++) {
					if (msg.live[i]) {
						flag = false;
						break;
					}
				}
				if (flag) {
					closesocket(msg.clientSocket);
					exit(0);
				}
			}
		}
		else {
			std::cout << "Wrong Expression!" << std::endl;
		}
	}
}

bool Server::RunServer(){
	// 设定服务器的ssid等
	int on = 1;
	char a[6] = "close";
	DisconnectMessage msg(a, live, serverSocket, -1);
	closeThread = new std::thread(CloseServerThread, msg);
	// 创建socket
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket < 0) {
		printf("socket creation error\n");
		return false;
	}
	else printf("socket successful\n");
	// 设置socketopt，SO_REUSEADDR表示这个ssid即使被关闭了也可重用，复合客户端需要和服务器保持连接的逻辑
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,(char*)&on, sizeof(on));
	//将  
	int b = bind(serverSocket, (sockaddr*)&serverChannel,sizeof(serverChannel));
	if (b < 0) {
		printf("bind error\n");
		return false;
	}
	else printf("successfully bind\n");
	//监听  
	int l = listen(serverSocket, 1);
	if (l < 0) {
		printf("listen failed\n");
		return false;
	}
	int len = sizeof(serverChannel);
	int clientSocket;
	//服务器等待连接 
	while (1) {
		printf("Waiting for client to enter...\n");
		//试图接受一个连接，这个连接是阻塞的，没有新的连接进入会一直阻塞在那里
		clientSocket = accept(serverSocket, (sockaddr*)&serverChannel, &len);
		//如果在阻塞的过程中出现了错误，则返回错误
		if (clientSocket < 0) {
			printf("ACCEPT_ERROR\n");
		}
		else {
			// 接收成功，也就算连接成功了
			printf("Connection successful!\n");
			memset(buffer, 0, sizeof(buffer));
			int ret;
			// 此时客户端必然会发来请求（页面或文本），这时使用recv阻塞函数监听这一请求
			ret = recv(clientSocket, buffer, BUFFER_SIZE, 0);
			if (ret == SOCKET_ERROR) {
				printf("Temporarily fail to receive due to socket error(ret<0)\n");
			}
			// 出现0，连接没有出现异常，一般情况下是客户端主动关闭连接
			else if (ret == 0) {
				printf("A sudden disconnection occur(ret=0)\n");
			}
			else {
				printf("Reception Sucessful\n");
				for (int i = 0; i < MAX_THREAD; i++) {
					if (!live[i]) {
						live[i] = true;
						// 构造处理特定客户端信息的主线程
						Message msg(buffer, &live[i], clientSocket, i);
						t[i] = new std::thread(ProcessMsg, msg);
						break;
					}
				}
			}
		}
	}
}

