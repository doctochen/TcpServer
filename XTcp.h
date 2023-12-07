#pragma once
#include <string>

class XTcp
{
public:
	XTcp();
	virtual ~XTcp();
	int createSocket();
	bool bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char* buf, int bufSize);
	int Send(const char* buf, int sendSize);

	int sock = 0;
	unsigned short port = 0;
	std::string ip;
};

