#include <cstdio>
#include<map>
#include<event.h>
#include"tcpserver.h"

map<int, struct event*> aaa;
int main()
{
	const char* ip = "127.0.0.1";
	short port;
	cout << "please input port: ";
	cin >> port;
	int pth_num = 5;

	TcpServer *ser = new TcpServer(ip, port, pth_num);
	cout << "<==  TcpServer::TcpServer()  ==>" << endl;
	ser->Run();
	delete ser;
	return 0;
}