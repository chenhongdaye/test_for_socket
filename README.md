# test_for_socket
try to connect other pc by using socket.
server:create a socket,bind to its own addr;and keep doing this:listen the socket,when a connect request be received,accept          it, then create a new thread work on this connect.
服务器:每接受一个新连接,即创建一个新线程,处理这个连接的一切事情;
        每当有ID上线时,更新在线文件,在线文件:用户ID,用户Addr;
        每当有下线时,返回一个确认消息, 关闭当前 socket,更新在线文件,退出线程;
        发送文件:线程中,收到的信息中取出送往 Id, 在线文件中找出对应的 socket,向这个 socket发送信息;
  
client:also create a socket,then connect the server directly through a both_known addr;
        the client focus on the console`s input, when the key enter was push down ,send the message to server by the socket          which is connect to the server.
客户端:连接服务器,通过直接与服务器通信,实现间接的和另外客户端通信;
        每下线,事先在发送消息的明冷行,输入 "下线"或者"log out";收到server回复之后下线;
        发送信息的 struct mes 格式的约定:开头是发送往的id,接下来是消息内容;

the users id and password in the files,which is just created without sign up.
登录文件:存id 和 密码,事先存好的,不经过注册啥的;

the message send to the id which is not online for now ,server save it in another file ,the struct of the message which the           file saves has several attributes : send_id ,send_time.
储存离线信息文件: 信息结构中包含发送的信息,发送者id,发送方id,发送时间.

there is also a file ,which saves all users who online now ,which is update when an user is become online;
在线用户信息文件:保存着当前在线的所有用户id,Addr地址,服务器相应的socket(用于其他线程发送信息时使用);当发送给在线人信息时,通过此映射,找到地址,发送;


!!!!!!!!!!!!!!!!!attention: 往user文件写id和密码时候,严格按照 struct user写;
