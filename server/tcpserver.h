#pragma once
#include<vector>
#include<map>
#include<event.h>
#include"mypthread.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<memory.h>
#include<string>
#include<exception>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<json/json.h>
#include<sys/types.h>
#include<event.h>

#include<iostream>
using namespace std;

class TcpServer
{
public:
	TcpServer(char *ip, short port, int pthnum);
	
	~TcpServer();
	
	void Run();

	class pipe
	{
	public:
		pipe(int err[2])
		{
			arr[0] = err[0];
			arr[1] = err[1];
		}
		int arr[2];
	};

private:
	int listen_fd;	//�����׽���
	int pth_num;	//�����̸߳���
	struct event_base* base;

	vector<pipe> socket_pair;
	vector<Pthread*> pthread;
	map<int, int> pth_work_num; //���ں����߳̽�����fd�Ͷ�Ӧ���̼߳����ĸ���

	void GetSockPair();
	void GetPthread();

	friend void listen_cb(int fd, short event, void* arg);
	friend void sock_pair_cb(int fd, short event, void* arg);
};
