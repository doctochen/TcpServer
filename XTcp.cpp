#include "XTcp.h"
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

XTcp::XTcp()
{
#ifdef _WIN32
	static bool first = true;
	if (first)
	{
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws); // 初始化库
	}
	first = false;
#endif


}

XTcp::~XTcp()
{
}

int XTcp::createSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0); // 创建了一个socket
	if (sock == -1)
	{
		std::cout << "socket failed..." << std::endl;
	}
	std::cout << sock << std::endl;

	return sock;
}

bool XTcp::bind(unsigned short port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(0);

	if (::bind(sock, (sockaddr*)&addr, sizeof(addr)) != 0)
	{
		std::cout << "bind port " << port << " failed..." << std::endl;
		return false;
	}
	std::cout << "bind port " << port << " success..." << std::endl;

	listen(sock, 10); // 监听
	return true;
}

XTcp XTcp::Accept()
{
	XTcp tcp;
	sockaddr_in caddr;
	socklen_t len = sizeof(caddr);

	int client = accept(sock, (sockaddr*)&caddr, &len); // 获取传入连接请求，返回新的连接的套接字描述符
	if (client <= 0) return tcp;
	std::cout << "client " << client << " success" << std::endl;

	tcp.ip = inet_ntoa(caddr.sin_addr); // ip地址
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	std::cout << tcp.ip << ' ' << tcp.port << std::endl;

	return tcp;
}

void XTcp::Close()
{
	if (sock <= 0) return;

	closesocket(sock);
}

int XTcp::Recv(char* buf, int bufSize)
{
	return recv(sock, buf, bufSize, 0);
}

int XTcp::Send(const char* buf, int sendSize)
{
	int sendedSize = 0;
	while (sendedSize != sendSize)
	{
		int len = send(sock, buf + sendSize, sendSize - sendSize, 0);
		if (len <= 0) break;
		sendedSize += len;
	}
	return sendedSize;
}
