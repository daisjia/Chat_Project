#ifndef FUN_H
#define FUN_H

#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<error.h>
#include<string.h>
#include<string>
#include<memory>
#include<stdio.h>
#include<signal.h>
#include<json/json.h>
#include<map>
#include"public.h"
using namespace std;

void Register(int fd);

void Login(int fd);

void GetList(int fd);

void TalkToOne(int fd);

void TalkToGroup(int fd);

void Exit(int fd);

void* PthRun(void* arg);

#endif