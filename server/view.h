#pragma once
#include<string>
#include<iostream>
#include<json/json.h>
#include<stdio.h>
#include"mysql.h"
#include<event.h>
#include<sys/socket.h>
#include <string.h>

using namespace std;
extern Mysql Mysql_server;
extern map<int, struct event*> aaa;


class View
{
public:
	virtual void process(int fd, string json) {};
	virtual void response() {};
	int _fd;
	string str;
};

class Exit : public View
{
public:
	void process(int fd, string json)
	{
		_fd = fd;
		bool success = true;
		Json::Value root;
		Json::Reader read;

		cout << "exit json: " << json << endl << endl;

		if (-1 == read.parse(json, root))
		{
			cout << "json parse fail ==>> class Exit : public View" << endl;
			return;
		}
		char name[20];
		strcpy(name, root["name"].asString().c_str());

		char cmd[100] = "delete from online where name = '";
		strcat(cmd, name);
		strcat(cmd, "';");
		cout << endl << "exit json" << json << endl;
		//cout << "exit cmd" << cmd << endl;

		if (mysql_real_query(Mysql_server.mpcon, cmd, strlen(cmd)))
		{
			cout << "exit fail ==>> class Exit : public View" << endl;
			return;
		}

		str = name;
		str.append(" exit~ ");
		cout << "--> user: " << name << " offline!" << endl;
		event_free(aaa[fd]);
	}

	void response()
	{
		send(_fd, str.c_str(), strlen(str.c_str()), 0);
	}
};

class GetList : public View
{
public:
	void process(int fd, string json)
	{
		
	}

	void response()
	{
		send(_fd, str.c_str(), strlen(str.c_str()), 0);
	}
};


class Login : public View
{
public:
	void process(int fd, string json)
	{
		_fd = fd;

		Json::Value root;
		Json::Reader read;

		if (-1 == read.parse(json, root))
		{
			cout << "json parse fail ==>> class Login : public View" << endl;
			return;
		}
		char pw[20];
		strcpy(pw, root["pw"].asString().c_str());
		strcpy(name, root["name"].asString().c_str());

		char cmd[100] = "select * from user where name = '";
		strcat(cmd, name);
		strcat(cmd, "';");
		cout << "login json : " << json << endl << endl;
		//		cout << "login cmd : " << cmd << endl;

		if (mysql_real_query(Mysql_server.mpcon, cmd, strlen(cmd)))
		{
			cout << "exit fail ==>> class Exit : public View" << endl;
			return;
		}

		Mysql_server.mp_res = mysql_store_result(Mysql_server.mpcon);
		if (Mysql_server.mp_row = mysql_fetch_row(Mysql_server.mp_res))	//用户存在
		{
			char cmd2[100] = "select * from online where name = '";
			strcat(cmd2, name);
			strcat(cmd2, "';");
			//cout << "cmd2 : " << cmd2 << endl;
			mysql_real_query(Mysql_server.mpcon, cmd2, strlen(cmd2));
			Mysql_server.mp_res = mysql_store_result(Mysql_server.mpcon);
			//cout << "cmd2 : " << cmd2 << endl;
			if (Mysql_server.mp_row = mysql_fetch_row(Mysql_server.mp_res))
			{
				str = "login fail!";
				return;
			}
			else
			{
				//将fd,name加入到online表中
				char ffd[16] = { 0 };
				char cmd3[100] = "insert into online values ('";		//insert into online values ('jiadai');
			//	sprintf(ffd, "%d", fd);
			//	strcat(cmd3, ffd);
			//	strcat(cmd3, "' , '");
				strcat(cmd3, name);
				strcat(cmd3, "');");
				//cout << "cmd3 : " << cmd3 << endl;

				if (mysql_real_query(Mysql_server.mpcon, cmd3, strlen(cmd3)))
				{
					cout << "insert 1 fail ==>> class Login : public View" << endl;
					return;
				}
				cout << "-->" << "user: " << name << " online!" << endl;
				str = "login success!";
				return;
			}
		}
		cout << "finlly" << endl;
		str = "login fail!";
	}

	void response()
	{
		send(_fd, str.c_str(), strlen(str.c_str()), 0);
		/*char cmd11[100] = "select * from offline where name = '";
		strcat(cmd11, name);
		strcat(cmd11, "';");

		mysql_real_query(Mysql_server.mpcon, cmd11, strlen(cmd11));
		Mysql_server.mp_res = mysql_store_result(Mysql_server.mpcon);

		if (Mysql_server.mp_row = mysql_fetch_row(Mysql_server.mp_res))
		{
			cout << "offline message : " << Mysql_server.mp_row[1] << endl;
			send(_fd, Mysql_server.mp_row[1], strlen(Mysql_server.mp_row[1]), 0);
		}

		char cmd13[100] = "DELETE FROM offline WHERE name='";
		strcat(cmd13, name);
		strcat(cmd13, "';");
		cout << "offline cmd" << cmd13 << endl;
		mysql_real_query(Mysql_server.mpcon, cmd13, strlen(cmd13));*/
	}
private:
	char name[20];
};

class Register : public View
{
public:
	void process(int fd, string json)
	{
		_fd = fd;
		Json::Value root;
		Json::Reader read;
		if (-1 == read.parse(json, root))
		{
			cout << "json parse fail ==>> class Register : public View" << endl;
			return;
		}

		char name[20] = { 0 };
		strcpy(name, root["name"].asString().c_str());
		char pw[20] = { 0 };
		strcpy(pw, root["pw"].asString().c_str());

		//在数据库中查找有没有重名的name
		char cmd[100] = "select * from user where name = '";
		strcat(cmd, name);
		strcat(cmd, "';");
		mysql_real_query(Mysql_server.mpcon, cmd, strlen(cmd));
		Mysql_server.mp_res = mysql_store_result(Mysql_server.mpcon);

		cout << "register json:" << json << endl << endl;
		if (!(Mysql_server.mp_row = mysql_fetch_row(Mysql_server.mp_res)))
		{
			char cmd2[100] = "insert into user values ('";
			strcat(cmd2, name);
			strcat(cmd2, "', '");
			strcat(cmd2, pw);
			strcat(cmd2, "');");

			if (mysql_real_query(Mysql_server.mpcon, cmd2, strlen(cmd2)))
			{
				cout << "register insert fail ==>> class Register : public View" << endl;
				str = "register fail!";
				return;
			}

			str = "register success!";
		}
		else
		{
			str = "register fail!";
		}
	}

	void response()
	{
		send(_fd, str.c_str(), strlen(str.c_str()), 0);
	}
};

class TalkGroup : public View
{
public:
	void process(int fd, string json)
	{
		
	}

	void response()
	{
		send(_fd, str.c_str(), strlen(str.c_str()), 0);
	}
};

class TalkOne : public View
{
public:
	void process(int fd, string json)
	{
		
	}

	void response()
	{
		send(_fd, str.c_str(), strlen(str.c_str()), 0);
	}
};