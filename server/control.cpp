#include "control.h"
#include "view.h"
#include"public.h"
Control::Control()
{
	mp.insert(make_pair(MSG_TYPE_REGISTER, new Register()));
	mp.insert(make_pair(MSG_TYPE_LOGIN, new Login()));
	mp.insert(make_pair(MSG_TYPE_GET_LIST, new GetList()));
	mp.insert(make_pair(MSG_TYPE_TALK_TO_ONE, new TalkOne()));
	mp.insert(make_pair(MSG_TYPE_TALK_TO_GROUP, new TalkGroup()));
	mp.insert(make_pair(MSG_TYPE_EXIT, new Exit()));
}

Control::~Control()
{
}

void Control::process(int fd, string json)
{
	Json::Value root;
	Json::Reader read;

	if (-1 == read.parse(json, root))
	{
		cout << "json parse fail ==>> void Control::process(int fd, string json)" << endl;
		return;
	}
	
	mp[root["type"].asInt()]->process(fd, json);
	mp[root["type"].asInt()]->response();
}
