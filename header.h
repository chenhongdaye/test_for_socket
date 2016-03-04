
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
#define SUCC 1;
#define FAIL 0;
#define SERVER_IP "....."
#define SERVER_PORT ..... //短整形
#define ONLINE "....."
#define OFFLINE "....."
#define USRS "....."


struct message //发送的消息or离线文件  相应的ID 为接收或者发送;
{
  char id[ID_LEN]; //最长10
  char message[MES_LEN]; //最长50
  time_t time;
};

struct online //在线文件夹
{
  char id[ID_LEN];
  int sockfd;
  pthread_mutex_t mutex; //新增加, 用于保证发送消息原子性;
};
typedef struct online thread_argu;
/*
struct send //用于 传递给服务器中 线程的参数
{
  int sockfd;
  pthread_mutex_t mutex;
  char id_from[ID_LEN]; //当前id
  char id_to[ID_LEN]; 
};
*/

struct users //用户信息 用户名和密码
{
  char id[ID_LEN];
  char key[KEY_LEN];
};

typedef struct message login;

login string_op_login(char *string)  //从登录信息分离出消息
{
  return ....;
}

struct message string_op(char *input) ////////////待 从发送消息分离出 信息
{
  return ....;
}

int init_user_file(char filename)
{
  filename=USRS;
  //.........................
}



