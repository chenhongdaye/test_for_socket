#include"header.h"

struct message string_op(char*);

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
  
  puts("连接成功: \n");
  puts("当前在线的人有:\n");
  FILE *f_online=fopen(ONLINE,"r");
  struct online usr;
  int count;
  while((count=fread((void*)(&usr),sizeof(struct online),1,f_online))==1)  
  {
    puts((struct online)usr->id);
  }
  puts("\n 现在可以输入了 :(格式  id:message)\n ");
  struct message message;
  char *input;
  while(1)
  {
    gets(input); //以字串形式输入;
    struct message buf=string_op(input);
    if(send(socketfd,(void*)buf,MES_LEN,0)==-1)
    {
      puts("send failed \n");
    }
    puts("\n发送成功\n");
  }
  return 0;
}
