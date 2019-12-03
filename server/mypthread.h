#pragma once
#include<pthread.h>
#include<map>
#include<event.h>
#include<iostream>
using namespace std;
#include"control.h"

class Pthread
{
public:
	Pthread(int fd);
	~Pthread();

private:
	int sockfd;
	struct event_base* base;
	map<int, struct event*> event_map;
	pthread_t pthid;

	friend void* pthread_run(void* arg);
	friend void sock_pair_1_cb(int fd, short event, void* arg);
};

void *pthread_run(void* arg);
void sock_pair_1_cb(int fd, short event, void* arg);
void client_cb(int fd, short event, void* arg);