#include "tcpserver.h"

void listen_cb(int fd, short event, void* arg)
{
	TcpServer* mythis = (TcpServer*)arg;
	struct sockaddr_in caddr;
	socklen_t len = sizeof(caddr);

	int clifd = accept(fd, (struct sockaddr*) & caddr, &len);

	if (-1 == clifd)
	{
		cout << "accept fail ==>> void listen_cb(int fd, short event, void* arg)" << endl;
		return;
	}

	//查找当前监听数量最少的子线程
	int min = 65535;
	int tmpfd = 0;

	map<int, int>::iterator it = mythis->pth_work_num.begin();
	for (; it != mythis->pth_work_num.end(); ++it)
	{
		if (it->second < min)
		{
			min = it->second;
			tmpfd = it->first;
		}
	}

	char buff[16] = { 0 };
	sprintf(buff, "%d", clifd);

	if (write(tmpfd, buff, sizeof(buff)) > 0)
	{
		cout << "<== write success ==>" << endl;
	}
	else
	{
		cout << "write error ==> void listen_cb(int fd, short event, void* arg)" << endl;
	}
}

void sock_pair_cb(int fd, short event, void* arg)
{
	cout << "=================================" << endl;
	cout << "<==  in sock_pair_cb()  ==>" << endl;

	TcpServer* mythis = (TcpServer*)arg;
	int num = 0;
	char buff[10] = { 0 };
	if (read(fd, buff, 9) > 0)
	{
		cout << "read success !" << endl;
	}
	else
	{
		cout << "read error ==>> void sock_pair_cb(int fd, short event, void* arg)" << endl;
	}

	num = atoi(buff);
	mythis->pth_work_num[fd] = num;

	cout << "out sock_pair_cb()" << endl;
	cout << "=================================" << endl;
}



TcpServer::TcpServer(char* ip, short port, int pthnum)
{
	pth_num = pthnum;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listen_fd)
	{
		cout << "listen_fd fail ==>> TcpServer(char* ip, short port, int pthnum)" << endl;
		return;
	}
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(p);
	cout << "<==  saddr init success  ==>" << endl;

	int res = bind(listen_fd, (struct sockaddr*) & saddr, sizeof(saddr));
	if (res == -1)
	{
		cout << "bind fail ==>> TcpServer(char* ip, short port, int pthnum)" << endl;
		return;
	}

	res = listen(listen_fd, 20);
	if (res == -1)
	{
		cout << "listen fail ==>> TcpServer(char* ip, short port, int pthnum)" << endl;
		return;
	}

	base = event_base_new();

	struct event* ev = event_new(base, listen_fd, EV_READ | EV_PERSIST, listen_cb, this);
	if (ev != NULL)
	{
		cout << "<== event create success! ==>" << endl;
	}
	else
	{
		cout << "event new fail ==>> TcpServer(char* ip, short port, int pthnum)" << endl;
	}

	event_add(ev, NULL);
	cout << "<== tcpsever listen cb already carry out ==>" << endl;
}

TcpServer::~TcpServer()
{
}

void TcpServer::Run()
{
	cout << "=============================================" << endl;
	cout << "<==  in run  ==>" << endl;

	GetSockPair();
	GetPthread();

	for (int i = 0; i < pth_num; i++)
	{
		struct event* ev = event_new(base, socket_pair[i].arr[0], EV_READ | EV_PERSIST, sock_pair_cb, this);
		if (NULL == ev)
		{
			return;
		}
		else
		{
			event_add(ev, NULL);
		}
	}

	cout << "Run() for already carry out" << endl;
	event_base_dispatch(base);
}

void TcpServer::GetSockPair()
{
	for (int i = 0; i < pth_num; i++)
	{
		int pair[2];
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1)
		{
			cout << "socketpair create fail ==>> void TcpServer::GetSockPair()" << endl;
			return;
		}

		pipe pi(pair);
		socket_pair.push_back(pi);
		pth_work_num.insert(make_pair(pi.arr[0], 0));
	}
}

void TcpServer::GetPthread()
{
	for (int i = 0; i < pth_num; i++)
	{
		pthread.push_back(new Pthread(socket_pair[i].arr[1]));
	}
}
