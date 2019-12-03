#pragma once
#include <string>
#include"view.h"

class Control
{
public:
	Control();

	~Control();

	void process(int fd, string json);
private:
	map<int, View*> mp;
};
