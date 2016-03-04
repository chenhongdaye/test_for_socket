#include"header.h"



int main(void)
{
  int sockfd;
  struct sockaddr_in server;
  
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    puts("socket is failed! \n");
    exit(1);
  }
  
  server.sin_family=AF_INET;
  server.sin_port=htons(SERVER_PORT);
  server.sin_addr.s_addr=inet_addr(SERVER_IP);
  
  if(bind(sockfd,(struct sockaddr*)(&server),sizeof(struct sockaddr))==-1)
  {
    puts("bind is failed !\n");
    exit(1);
  }
  
  while(1)
  {
    if(listen(sockfd,10)==-1)
    {
      puts("the socket cannot listen !\n");
      exit(1);
    }
    
    int new_sockfd;
    struct sockaddr client;  //不使用此地址,直接写成原始地址;
    int length=sizeof(struct sockaddr);
    
    if((new_sockfd=accept(sockfd,&client,&length)==-1)
    {
      puts(" accept is falied !\n")
      exit(1);
    }
    
    label1:
    login login_message;
    if(recv(new_sockfd,(void*)(&login_message),sizeof(login),0)==-1)
    {
      put("!!!!!!!!!!!\n");
      exit(1);
    }
    
    FILE *usr_file;
    if(usr_file=(fopen(USRS,"r"))==NULL)
    {
      puts(" 用户文件打开失败 请检查 \n");
      exit(1);
    }
    
    login compare;
    int succ=0;
    while((fread((void*)(&compare),sizeof(login),1,file_usr))==1)
    {
      if(login_message.id==compare.id&&login_message.key==compare.key)
      {///表明登录信息正确;
        int a=SUCC;
        if(send(new_sockfd,(void*)(&a),sizeof(int))==-1)
        {
          puts(" send shibai 1111\n");
          exit(1);
        }
        succ=1;
        break;
      }
    }
    if(succ==0)
    {//表明登录信息 错误
      int a=fail;
      send(new_sockfd,(void*)(&a),sizeof(int));
      goto label1;
    }
    
    // only if succ==1 才执行以后的.
    FILE *file_online;
    pthread_mutex_t mutex;
    pthread_mutex_init(mutex);
    if((file_online=(fopen(ONLINE,"a+")))!=NULL) //追加方式写进去,更新在线文件,,,,,, 向当前在线用户发送上线消息;
    {
      struct online a;
      a.id=login_message.id;
      a.sockfd=new_sockfd;
      a.mutex=mutex;
      if(fwrite((void*)(&a),sizeof(struct online),1)==0)
      {
        puts(" 写入文件 失败!!!!\n");
        exit(1);
      }
    }
    struct online temp;
    while(fread((void*)(&temp),sizeof(struct online),1,file_online)==1)
    {
      pthread_mutex_lock(temp.mutex);
      if(send(temp.sockfd,(void*)(temp.id),ID_LEN*sizeof(char))==-1)
      {
        
      }
      pthread_mutex_unlock(temp.mutex);
    }
    fclose(file_online);
    
    //创建线程  负责此连接的通信;
    pthread_t thread;///////////////////////////////////////////////////////////////////未;
    if(pthread_create(&thread,,)==-1)
    {
      
    }
    
  }
  
  void connect_handle(struct send temp)
  {
    int sockfd=temp.sockfd;
    pthread_mutex_t mutex=temp.mutex;
    char id_from[ID_LEN]=temp.id_from;
    char id_to[ID_LEN]=temp.id_to;
    char *buf;
    struct message message;
    time_t timer;
    while(1)
    {
      pthread_mutex_lock(mutex);
      if(recv(sockfd,(void*)(&message),sizeof(struct message),0)==-1)
      {
        puts("sth is wrong!!1111111111111111111\n");
        exit(1);
      }
      pthread_mutex_unlock(mutex);
      char id[ID_LEN]=message.//////////////////////////////////////////////
      message.id=id_from;
      
      
    }
  }
  
  
  
  return 0;
}
