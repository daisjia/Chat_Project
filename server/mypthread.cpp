#include "mypthread.h"
#include<cstdlib>
#include<string.h>
#include"control.h"

static Control control_server;
extern map<int, struct event*> aaa;

Pthread::Pthread(int fd)
{
	sockfd = fd;
	base = event_base_new();

	int res = pthread_create(&pthid, NULL, pthread_run, this);
	cout << "Pthread id : " << pthid << " create success!" << endl;
	if (res != 0)
	{
		cout << "pthread create fail ==>> Pthread(int fd)" << endl;
	}
}

Pthread::~Pthread()
{
}

void* pthread_run(void* arg)
{
	Pthread* mythis = (Pthread*)arg;
	cout << "pthread recv sockpair[1] = " << mythis->sockfd << endl;
	struct event* sock_event = event_new(mythis->base, mythis->sockfd, EV_READ | EV_PERSIST, sock_pair_1_cb, arg);
	event_add(sock_event, NULL);
	event_base_dispatch(mythis->base);
}

void sock_pair_1_cb(int fd, short event, void* arg)
{
	//cout << "<=============================================>" << endl;
	//cout << "in sock_pait_1_cb" << endl;

	Pthread* mythis = (Pthread*)arg;

	char buff[20] = { 0 };

	if (read(mythis->sockfd, buff, 19) < 0)
	{
		cout << "recv error  ==>> void sock_pair_1_cb(int fd, short event, void* arg)" << endl;
	}

	int clifd = atoi(buff);
	struct event* listen_event = event_new(mythis->base, clifd, EV_READ | EV_PERSIST, client_cb, arg);
	event_add(listen_event, NULL);

	cout << "thread: " << mythis->pthid << " deal with: " << clifd << endl;
	
	mythis->event_map.insert(make_pair(clifd, listen_event));
	aaa.insert(make_pair(clifd, listen_event));

	//给主线程回复当前监听的客户端的数量
	int num = mythis->event_map.size();
	//char abuf[16] = { 0 };
	//sprintf(buff, "%d", num);
	if (write(mythis->sockfd, (void *)&num, 4) < 0)
	{
		cout<<"write error ==> void sock_pair_1_cb(int fd, short event, void* arg)"<<endl;
	}
//	cout << "out sock_pair_1_cb()" << endl;
//	cout << "<=============================================>" << endl;
}

void client_cb(int fd, short event, void* arg)
{
	char buff[128] = { 0 };
	Pthread* mythis = (Pthread *)arg;
	if (recv(fd, buff, 127, 0) == -1)
	{
		cout << "recv error ==>> void client_cb(int fd, short event, void* arg)" << endl;
		return;
	}
//	else
//	{
////		cout << "last recv success" << endl;
//	}

	control_server.process(fd, buff);
}
