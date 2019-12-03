#include "fun.h"
#include<stdio.h>
#include<istream>
string myname;
void Register(int fd)
{
	string name;
	string pw;

	cout << "please input your name:  ";
	cin >> name;
	cout << "please input your passwd:  ";
	cin >> pw;

	Json::Value val;
	val["type"] = MSG_TYPE_REGISTER;
	val["name"] = name;
	val["pw"] = pw;

	int res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
	if (res == -1)
	{
		cout << "send fail! ==>> void Register(int fd)" << endl;
		return;
	}

	char buff[256] = { 0 };
	res = recv(fd, (void *)buff, 255, 0);
	if (res > 0)
	{
		if (strncmp(buff, "register success!", 16) == 0)
		{
			cout << "-----register sucess!-----" << endl;
			return;
		}
	}
	cout << "-----register fail!-----" << endl;
}

void Login(int fd)
{
	bool flag = false;
	string name;
	string pw;

	cout << "please input your name:  ";
	cin >> name;
	cout << "please input your passwd:  ";
	cin >> pw;

	Json::Value val;
	val["type"] = MSG_TYPE_LOGIN;
	val["name"] = name;
	val["pw"] = pw;

	int res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
	if (-1 == res)
	{
		cout << "send fail ==>> void Login(int fd)" << endl;
		return;
	}

	char buff[256] = { 0 };
	res = recv(fd, (void *)buff, 255, 0);
	if (res > 0)
	{
		if (strncmp(buff, "login success!", 13) == 0)
		{
			cout << "----- \033[35m" << name << "\033[0m login sucess!-----" << endl;
			flag = true;
		}
		else
		{
			cout << "<========login fail!========>" << endl;
		}
	}

	if (flag)
	{
		myname = name;
		map<int, void(*)(int)> mp;
		mp.insert(make_pair(1, GetList));
		mp.insert(make_pair(2, TalkToOne));
		mp.insert(make_pair(3, TalkToGroup));
		mp.insert(make_pair(4, Exit));
		while (true)
		{
			cout << "<==  1.get list    2.talk to one    3.talk to group    4.exit  ==>" << endl;
			//cout << "====================================================================" << endl;
			cout << "--> please input choice: ";
			int choice;
			cin >> choice;
			if (choice < 0 || choice > 4)
			{
				cout << "--> input error, please reinput: ";
				continue;
			}

			//启动一个线程专门打印消息
			pthread_t id;
			pthread_create(&id, NULL, PthRun, (void*)fd);
			
			mp[choice](fd);
			break;
		}
	}
}

void GetList(int fd)
{
	Json::Value val;
	val["type"] = MSG_TYPE_GET_LIST;
	val["name"] = myname;

	int res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
	if (res == -1)
	{
		cout << "send fail ==>> void GetList(int fd)" << endl;
		return;
	}
	pthread_t id;
	pthread_create(&id, NULL, PthRun, (void*)fd);
	pthread_join(id, NULL);
}

void TalkToOne(int fd)
{
	GetList(fd);
	string hername;
	cout << "---> please choose a friend: ";
	cin >> hername;

	char buff[256] = { 0 };
	cout << "-->input:";
	cin.ignore();
	cin.getline(buff, 256);

	Json::Value val;
	val["type"] = MSG_TYPE_TALK_TO_ONE;
	val["name"] = myname;
	val["hername"] = hername;
	val["message"] = buff;
	int res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
	if (res == -1)
	{
		cout << "send fail ==>> void TalkToOne(int fd)" << endl;
		return;
	}
}

void TalkToGroup(int fd)
{
	char hername[128] = { 0 };
	char sendbuf[256] = { 0 };
	cin.ignore();
	cout << "---> choose friends use ';' to divide: ";
	cin.getline(hername, 128);

	cout << "-->input: ";
	cin.getline(sendbuf, 256);
	Json::Value val;
	val["type"] = MSG_TYPE_TALK_TO_GROUP;
	val["name"] = myname;
	val["hername"] = hername;
	val["message"] = sendbuf;

	int res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
	if (res == -1)
	{
		cout << "send fail ==>> void TalkToGroup(int fd)" << endl;
		return;
	}
}

void Exit(int fd)
{
	Json::Value val;
	val["type"] = MSG_TYPE_EXIT;
	val["name"] = myname;

	int res = send(fd, val.toStyledString().c_str(), strlen(val.toStyledString().c_str()), 0);
	if (res == -1)
	{
		cout << "send fail ==>> void Exit(int fd)" << endl;
		return;
	}
	close(fd);
	exit(0);
}

void* PthRun(void* arg)
{
	int fd = (int)arg;
	char buff[1024] = { 0 };
	int n = recv(fd, (void *)buff, 1023, 0);
	while (n != 0)
	{
		cout << "message: " << buff << endl;
		n = recv(fd, buff, 1023, 0);
	}
}
