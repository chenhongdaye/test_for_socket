
#include<stdio.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h> //未知来源
#include<string.h>
#include<stdlib.h> //exit()
#include<time.h> //time_t
#define ID_LEN 10;
#define MES_LEN 50;
#define KEY_LEN 10;




struct message //发送的消息or离线文件
{
  char id[ID_LEN]; //最长10
  char message[MES_LEN]; //最长50
  time_t time;
};

struct online //在线文件夹
{
  char id[ID_LEN];
  int sockfd;
};

struct users //用户信息 用户名和密码
{
  char id[ID_LEN];
  char key[KEY_LEN];
};




