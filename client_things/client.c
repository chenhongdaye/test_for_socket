#include"header.h"

struct message string_op(char*);
void receive(struct online);

int main(void)
{
  int sockfd;
  struct sockaddr_in server;
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=inet_addr(SERVER_IP);
  server.sin_port=atons(SERVER_PORT);
  if((socketfd=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    puts("shit there is sth 1111 \n");
    exit(1);
  }
  
  if(connect(socketfd,(struct sockaddr*)(&server),sizeof(struct sockaddr))==-1) //阻塞连接
  {
    puts("connect failed!2222 \n");
    exit(1);
  }
  
  label1: //每当登录信息失败时,循环.........................................
  puts("清输入 自己的账户名和密码,形式 id:password");
  char * login_input;
  gets(login_input);
  login login_str=string_op_login(login_input);
  
  ///登录之后  先发送一条登录信息 接受服务器验证信息;
  if(send(socketfd,(void*)(&login_str),sizeof(login),0)==-1)
  {
    puts(" send failed \n");
    exit(1);
  }
  
  int success=0;
  if(recv(socketfd,(void*)(&success),sizeof(int))==-1)
  {
    puts(" shit recv failed \n");
    exit(1);
  }
  char *output=((int)success==1?"登录成功! \n":"账户/密码错误,请重新输入!\n");
  puts(output);
  if(success==0)
    goto label1;
  
  /*  未实现  发送全部在线人消息.
  puts("连接成功: \n");
  puts("当前在线的人有:\n");
  FILE *f_online=fopen(ONLINE,"r");
  struct online usr;
  int count;
  while((count=fread((void*)(&usr),sizeof(struct online),1,f_online))==1)  
  {
    puts((struct online)usr->id);
  }
  fclose(f_online);
  */
  
  puts("\n 现在可以输入了 :(格式  id:message)\n ");
  struct message message;
  char *input;
  
  pthread_t thread;
  struct online online1;
  online1.sockfd=socketfd;
  pthread_mutex_t mutex;
  pthread_mutex_init(mutex);
  online1.mutex=mutex;
  if(pthread_create(&thread,NULL,(void*)(&receive),(void*)(&online1))==-1)
  {
    puts(" 创建线程失败 \n");
    exit(1);
  }

  while(1)    //进入聊天  .......当前位置: 发送消息线程
  {
    gets(input); //以字串形式输入;
    struct message buf=string_op(input);
    if(input=="logout")
    {
      /////////////////////////////////////////////////////////////////////////////未
      break;
    }
    buf.time=time(NULL);
    pthread_mutex_lock(mutex);
    if(send(socketfd,(void*)buf,MES_LEN,0)==-1)
    {
      puts("send failed \n");
    }
    pthread_mutex_unlock(mutex);
    puts("\n发送成功\n 请输入:\n");
  }
  return 0;
}

void receive(struct online temp)
{
  int sockfd=temp.sockfd;
  pthread_mutex_t mutex=temp.mutex;
  struct message message;
  while(1)
  {
    pthread_mutex_lock(mutex);
    if(recv(sockfd,(void*)(&message),sizeof(struct message))==-1)
    {
      puts("");
      exit(1);
    }
    pthread_mutex_unlock(mutex);
    puts("from:"");
    puts(message.id);
    puts("\n message is:");
    puts(message.message);
    puts("\n");
    puts("time:")
    puts(ctime(message.time));
  }
}
