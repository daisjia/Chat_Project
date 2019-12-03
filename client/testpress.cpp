#include<iostream>
using namespace std;
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<json/json.h>
#include<stdio.h>
#include"public.h"
#include<pthread.h>

int port = 0;

void* login(void *arg)
{
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(port);
	saddr.sin_family = AF_INET;

	for (int i = 0; i < 100000; ++i)
	{
		sleep(1);
		int fd = socket(AF_INET, SOCK_STREAM, 0);

		int res = connect(fd, (struct sockaddr*) & saddr, sizeof(saddr));
		if (res == -1)
		{
			cout << "connect error!" << endl;
			return 0;
		}

		string name1("jiadai");
		char buff1[128] = {0};
		sprintf(buff1, "%d", i);
		string name2(buff1);
		string name = name1 + name2;
		string pw("123456");
		Json::Value val;
		val["type"] = MSG_TYPE_LOGIN;
		val["name"] = name;
		val["pw"] = pw;
		cout<<"-->login name: "<<name<<"  passwd: "<<pw<<endl;
		

		res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
		if (res == -1)
		{
			cout << "send error!" << endl;
			return 0;
		}

		char buff[256] = { 0 };
		res = recv(fd, buff, 255, 0);
		if (res == -1)
		{
			cout << "recv error!" << endl;
			return 0;
		}
		cout << buff << endl;
	}

	int a = 0;
	cin>>a;
}

int main()
{
	cout << "input port: ";
	cin >> port;

	pthread_t id;
	pthread_create(&id, NULL, login, NULL);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(port);
	saddr.sin_family = AF_INET;

	for (int i = 0; i < 100000; ++i)
	{
		sleep(1);
		int fd = socket(AF_INET, SOCK_STREAM, 0);

		int res = connect(fd, (struct sockaddr*) & saddr, sizeof(saddr));
		if (res == -1)
		{
			cout << "connect error!" << endl;
			return 0;
		}

		string name1("jiadai");
		char buff1[128] = {0};
		sprintf(buff1, "%d", i);
		string name2(buff1);
		string name = name1 + name2;
		string pw("123456");
		Json::Value val;
		val["type"] = MSG_TYPE_REGISTER;
		val["name"] = name;
		val["pw"] = pw;
		cout<<"-->register name: "<<name<<"  passwd: "<<pw<<endl;

		res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
		if (res == -1)
		{
			cout << "send error!" << endl;
			return 0;
		}

		char buff[256] = { 0 };
		res = recv(fd, buff, 255, 0);
		if (res == -1)
		{
			cout << "recv error!" << endl;
			return 0;
		}
		cout << buff << endl;


		string name11("jiadai");
		char buff11[128] = {0};
		sprintf(buff11, "%d", i);
		string name22(buff11);
		string namee = name11 + name22;
		string pww("123456");
		Json::Value vall;
		vall["type"] = MSG_TYPE_LOGIN;
		vall["name"] = namee;
		val["pw"] = pww;
		cout<<"-->login name: "<<namee<<"  passwd: "<<pww<<endl;
		

		res = send(fd, vall.toStyledString().c_str(), strlen(vall.toStyledString().c_str()), 0);
		if (res == -1)
		{
			cout << "send error!" << endl;
			return 0;
		}

		char bufff[256] = { 0 };
		res = recv(fd, bufff, 255, 0);
		if (res == -1)
		{
			cout << "recv error!" << endl;
			return 0;
		}
		cout << bufff << endl;

	}

	int a;
	cin>>a;
	pthread_join(id, NULL);

	return 0;
}
