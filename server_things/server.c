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
    if(file_usr=(fopen(USRS,"r"))==NULL)
    {
      puts(" 用户文件打开失败 请检查 \n");
      exit(1);
    }
    
    struct users compare;
    int succ=0;
    while((fread((void*)(&compare),sizeof(struct users),1,file_usr))==1)
    {
      char key_temp[KEY_LEN];
      memcpy(key_temp,(&login_message)+ID_LEN*sizeof(char),KEY_LEN*sizeof(char));
      if(strcmp(login_message.id,compare.id)==0&&strcmp(key_temp,compare.key)==0)
      {///表明登录信息正确;
        struct message success;
        success.id={'1','1','1','1','1','1','1','1','1','1'};
        if(send(new_sockfd,(void*)(&a),sizeof(int))==-1)
        {
          puts(" send shibai 1111\n");
          exit(1);
        }
        succ=1;
        break;
      }
    }
    close(file_usr);
    if(succ==0)/////////////////////////////////////////////////////////////改.......................
    {//表明登录信息 错误
      int a=FAIL;
      send(new_sockfd,(void*)(&a),sizeof(int));
      goto label1;
    }
    
    // only if succ==1 才执行以后的.
    FILE *file_online;
    pthread_mutex_t mutex;
    pthread_mutex_init(mutex);
    if((file_online=fopen(ONLINE,"a+"))!=NULL) //追加方式写进去,更新在线文件,,,,,, 向当前在线用户发送上线消息;
    {
      struct online a;
      memcpy(a.id,login_message.id,10*sizeof(char));
      a.sockfd=new_sockfd;
      a.mutex=mutex;
      if(fwrite((void*)(&a),sizeof(struct online),1,file_online)==0)
      {
        puts(" 写入文件 失败!!!!\n");
        exit(1);
      }
    }
    struct message temp;  ///发布用户  有人上线
    memcpy(temp.id,a.id,10*sizeof(char));
    //char mes[MES_LEN];
    memcpy(temp.message,strcat(temp.id,"is online!\n"),MES_LEN*sizeof(char));
    temp.time=time(NULL);
    struct online online;
    memcpy(temp.id,a.id,10*sizeof(char));
    while(fread((void*)(&online),sizeof(struct online),1,file_online)==1)
    {
      pthread_mutex_lock(online.mutex);
      if(send(temp.sockfd,(void*)(&temp),sizeof(struct message)==-1)
      {
        puts("shit !!!\n ");
        exit(1);
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
  
  void connect_handle(thread_argu temp) ///////此处传递形参  用于线程
  {
    int sockfd=temp.sockfd;
    pthread_mutex_t mutex=temp.mutex;
    char id_from[ID_LEN];
    memcpy(id_from,temp.id,MES_LEN*(sizeof(char)));
    struct message message; //用于发送的消息
    FILE *online_file;
    while(1)
    {
      pthread_mutex_lock(mutex);
      if(recv(sockfd,(void*)(&message),sizeof(struct message),0)==-1)
      {
        puts("sth is wrong!!1111111111111111111\n");
        exit(1);
      }
      pthread_mutex_unlock(mutex);
      char id_to[ID_LEN];
      memcpy(id_to,message.id,MES_LEN*sizeof(char));///发往的用户  用于在线文件中检索 socket和 thread
      memcpy(message.id,id_from,MES_LEN);  ///只需要改动接收到信息的 id部分;
      if((file_online=fopen(ONLINE,"r"))==NULL)
      {
        puts(" 打开文件失败 \n");
        exit(1);
      }
      int count=0;
      struct online usr;
      int label=0;
      int sockfd;
      pthread_mutex_t mutex;
      while((count=fread((void*)(&usr),sizeof(struct online),1,file_online))==1)
      {
        if(usr.id==id_to)  //发送往id 在线
        {
          sockfd=usr.sockfd;
          mutex=usr.mutex;
          label=1;
          break;
        }
      }
      close(file_online);
      
      if(label==1) //在线发送;
      {
        pthread_mutex_lock(mutex);
        if(send(sockfd,(void*)(&message),sizeof(struct message),0)==-1)
        {
          puts(" failed !\n");
          exit(1);
        }
        pthread_mutex_unlock(mutex);
      }
      
      else  //离线 写入离线文件;
      {
        FILE *offline;
        /////////////////////////////////////////////////////////////////未实现上线接收离线消息;
        if((offline=fopen(OFFLINE,"a+"))==NULL)   //更新离线消息文件
        {
          puts(" open file failed! \n");
          exit(1);
        }
        if(fwrite((void*)(&message),sizeof(struct message),1,offline)==0)
        {
          puts("写入失败 \n");
          exit(1);
        }
        close(offline);
      }
    }
  }
  
  
  
  return 0;
}
