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
    
    FILE *file_usr;
    if(file_usr=(fopen(USRS,"r"))==NULL)
    {
      puts(" 用户文件打开失败 请检查 \n");
      exit(1);
    }
    //char id[ID_LEN];
    //char key[KEY_LEN];
    //id 直接就是 message中id; key需要从message中提取;
    //strncpy(key,login_message.message,KEY_LEN);
    struct users compare;
    int succ=0;
    while((fread((void*)(&compare),sizeof(struct users),1,file_usr))==1)
    {
      //char key_temp[KEY_LEN];
      //memcpy(key_temp,(&login_message)+ID_LEN*sizeof(char),KEY_LEN*sizeof(char));
      if(strcmp(login_message.id,compare.id)==0&&strncmp(login_message.message,compare.key,KEY_LEN)==0)
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
    if(succ==0)
    {//表明登录信息 错误
      struct message fail;
      fail.id[0]='0';
      send(new_sockfd,(void*)(&fail),sizeof(struct message));
      goto label1;
    }
    
    //查看是否有离线消息;
    FILE* offline_mes;
    if((offline_mes=fopen(OFFLINE,"r"))==NULL)  //当有离线消息时, 关闭此文件,以 w+方式重新打开,删除离线消息;
    {
      puts(" 打开 离线文件失败! \n");
      exit(1);
    }
    struct message off;  //与compare 做比较
    int label_find=0;
    int count=0;
    struct message *temp=(struct message*)malloc(10*sizeof(struct message));//有必要在离线文件写入离线消息数;这假定最多10条;
    while(fread((void*)(&off),sizeof(struct message),1,offline_mes)==1)
    {
      ++count;
      memcpy(temp+count-1,&off,sizeof(struct message));
      if(strcmp(off.id,compare.id)==0)
      {
        label_find=1;
        --count;
        //break;
      }
      
    }
    fclose(offline_mes);
    if(label_find==0)
    {
      ////nothing;
      free(temp);
    }
    else
    {
      ///采用相应的消息格式
      if(send(new_sockfd,(void*)(&off),sizeof(struct message),0)==-1)
      {
        puts("shit thrtresrsers\n");
        exit(1);
      }
      FILE * update_off;
      if((update_off=fopen(OFFLINE,"w+"))==NULL)
      {
        rewind(update_off);
        //for(int i=0;i<count;++i)
        //{
        if(fwrite((void*)(temp+i),sizeof(struct message),count,update_off)!=count)
        {
          puts("shitshitshit111111111\n");
          exit(1);
        }
        //}
        fclose(update_off);
        free(temp);
      }
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
    pthread_t thread;
    thread_argu argu;
    argu.
    if(pthread_create(&thread,NULL,(void*)(&connet_handle),(void*)(&argu))==-1)
    {
      puts(" thread fail \n");
      exit(1);
    }
    pthread_detach(thread);
    
  }
  return 0;
}




void connect_handle(thread_argu temp) ///////此处传递形参  用于线程
{
  int sockfd=temp.sockfd;
  pthread_mutex_t mutex=temp.mutex;
  char id_from[ID_LEN];
  strncpy(id_from,temp.id,MES_LEN);
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
    char a[MES_LEN],b[MES_LEN];
    char *c="logout";
    //memcpy(a,c,6);
    //memcpy(b,mesage.mesage,6);
    if(strncmp(c,message.message,strlen(c))==0)
    {
      //用户下线消息;/////发送下线通知
      FILE * file_out;
      if((file_out=fopen(ONLINE,"r+"))==NULL) //读写方式打开
      {
        //int label=0;
        //int local=0;
        int label2=0;
        rewind(file_out);
        struct online temp;
        struct online save[10]; //存放用户信息
        while(fread((void*)(&temp),sizeof(struct online),1,file_out)==1)//未找到更好地方式更新文件;此处:全部读取然后覆盖;
        {
          //++label;
          ++label2;
          *(save+labe2-1)=temp;
          if(strcmp(id_from,temp.id)==0)
          {
            --label2;   //下次会覆盖;
            //local=labe2+1; //记录 位置;
          }
        }
        int count=label2+1;
        fclose(file_out);
        FILE* file_out2;
        if((file_out2=fopen(ONLINE,"w+"))==NULL) //此方式打开,原文件全部清空;
        {
          if(fwrite((void*)(&save[i]),sizeof(),count,file_out2)<count)
          {
            puts("there is sth wrong!!!更新文件!!!2222222222222\n");
            exit(1);
          }
        }
        fclose(file_out2);
      }
      close(sockfd);
      FILE *out_message;
      if((out_message=fopen(ONLINE,"r"))==NULL)
      {
        puts(" 这里毛病ksjdhgkjsdh \n");
        exit(1);
      }
      struct online send_to;
      while(fread((void*)(send_to),sizeof(struct online),1,out_message)==1)
      {
        struct message message_to;
        char *say="已下线\n";
        strncpy(message_to.message,say,strlen(say));
        //message_to.time=time(NULL); //原信息中已经确定;
        if(send(message_to.sockfd,(void*)(message_to),sizeof(struct message),0)==-1)
        {
          puts(" 发不出去了\n");
          exit(1);
        }
      }
      fclose(out_message);
      pthread_exit(0);
    } //结束 用户下线,更新文件,发送下线信息;
    
    
    char id_to[ID_LEN];
    memcpy(id_to,message.id,MES_LEN*sizeof(char));///发往的用户  用于在线文件中检索 socket和 thread
    //memcpy(message.id,id_from,MES_LEN);  ///只需要改动接收到信息的 id部分;  如果在线则改;
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
      if(strcmp(usr.id,id_to)==0)  //发送往id 在线
      {
        sockfd=usr.sockfd;
        mutex=usr.mutex;
        label=1;
        break;
      }
    }
    fclose(file_online);
    
    if(label==1) //在线发送;
    {
      pthread_mutex_lock(mutex);
      strncpy(message.id,id_from,MES_LEN);  ///只需要改动接收到信息的 id部分; 
      if(send(sockfd,(void*)(&message),sizeof(struct message),0)==-1)
      {
        puts(" failed !\n");
        exit(1);
      }
      pthread_mutex_unlock(mutex);
    }
      
    else  //离线 写入离线文件;
    {
      char *a="\nfrom:";
      char *b=strcat(a,message.id);
      strncpy(message.message+(MES_LEN-ID_LEN-(strlen(a))),b,strlen(b));
      FILE *offline;
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
