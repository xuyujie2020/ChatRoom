#ifndef SERVER_H
#define SERVER_H

#include "common.h"
#include<cstring>
class Server
{
public:
    Server();
    ~Server();
    /**
     * 1.设置套接字
     * 2.bind绑定端口
     * 3.设置监听
     * 
    */
    void Init();

    /**
     * 启动程序
    */
    void Start();
    /**
     * 移除客户端标识
    */
    void DeleteFd(int fd);
    /**
     * 添加一个客户端标识，表示一个客户端的连接
    */
    void InsertFd(int fd, int flag);
    /**
     * 处理客户端发过来的消息
     * 
    */
    void DealRreadFd(int fd);
    /**
     * 获取TCP连接
    */
    void GetClient(int fd);
    /***
     * 断开TCP连接
     * 
    */
    void PushClient(int fd);
    /**
     * 处理TCP消息，连接消息，客户端发送的消息等
    */
    void DealClientData(int fd);
   /**
    * 获取在线客户端
   */
  void GetClientOnline();
   /**
    * 获取消息
   */
  int GetData();
   /**
    * 向所有人发送消息
   */
  void SendMsgAll();
    /**
     * 向指定客户端发送消息
    */
   void SendMsgClient();

  
   

private:
    struct sockaddr_in server_address;
    struct pollfd *fds;
    char* buff=(char*)malloc(512);
    char* temp=(char*)malloc(512);
    int listenfd;
    int res;//状态
    int fdcount;
    struct DataBody*  trdata= new DataBody;

    std::list<int> onclient;

};




#endif