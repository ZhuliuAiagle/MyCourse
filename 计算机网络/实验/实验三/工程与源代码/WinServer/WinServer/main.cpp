#include"Server.h"

int main()
{
	Server myServer;
	if (!myServer.RunServer()) {
		printf("Run Server Failed!n");
	};
	system("pause");
}