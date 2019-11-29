#include"fun.h"


int fd = -1;

void fun(int sig)
{
	Exit(fd);
}

int main(int argc, char* argv[])
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd)
	{
		cout << "socket create fail ==>> main(int argc, char* argv[])" << endl;
		return 0;
	}

	//绑定信号处理函数
	signal(SIGINT, fun);
	string ip("127.0.0.1");
	string port;
	cout << "please input port:";
	cin >> port;

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(port.c_str()));
	saddr.sin_addr.s_addr = inet_addr(ip.c_str());

	int res = connect(fd, (struct sockaddr*) & saddr, sizeof(saddr));
	if (-1 == res)
	{
		cout << "connect fail ==>> int main(int argc, char* argv[])" << endl;
		return 0;
	}

	map<int, void(*)(int)> mp;
	mp.insert(make_pair(1, Register));
	mp.insert(make_pair(2, Login));
	mp.insert(make_pair(3, Exit));

	while (1)
	{
		cout << "=========================" << endl << endl;
		cout << "1.register     2.login     3.exit" << endl;
		cout << "please input: ";
		int choice;
		cin >> choice;
		if (choice == 1 || choice == 2 || choice == 3)
		{
			mp[choice](fd);
		}
		else
		{
			cout << "input error!" << endl;
		}
		cout << "=========================" << endl << endl;
	}

	return 0;
}