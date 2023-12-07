#ifdef _WIN32
#include <Windows.h>
#define socklen_t int
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define closesocket close
#endif

#include <iostream>
#include <cstring>
#include <thread>
#include "XTcp.h"

class TcpThread
{
public:
	void Main()
	{
		char buf[1024] = "";

		for (;;)
		{
			int recvLen = client.Recv(buf, sizeof(buf) - 1);
			if (recvLen < 0)
				break;
			buf[recvLen] = '\0';
			if (strstr(buf, "quit") != nullptr)
			{
				char re[] = "quit success\n";
				client.Send(re, strlen(re) + 1);
				break;
			}
			int sentLen = client.Send("ok\n", 4);

			std::cout << buf << std::endl;
		}

		client.Close();
		delete this;
	}

	XTcp client;
};

int main(int argc, char* argv[])
{
	unsigned short port = 8080;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}

	XTcp server;
	server.createSocket();
	server.bind(port);

	for (;;)
	{
		XTcp client = server.Accept();
		TcpThread* th = new TcpThread();
		th->client = client;
		std::thread sth(&TcpThread::Main, th);	//用th对象的Main方法，作为线程的入口函数
		sth.detach();	//分离子线程和主线程
	}

	server.Close();


	return 0;
}