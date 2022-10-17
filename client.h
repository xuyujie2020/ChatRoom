#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"
#include <thread>

class Client
{
    public:
        Client();
        /**
         * 初始化客户端
        */
        ~Client();
        void Init();
        /**
         * 启动客户端
        */
        void Start();
        /**
         * 等待线程接收消息
        */
        void RecvThread();
        /**
        * 对接收的消息编码
        */
        void EnCode();
        /**
         * 发送文件
        */
        void SendFile(std::string temp);
        /**
         * 发送数据包
        */
        void SendData();

    private:
        int res;
        struct sockaddr_in server_address; //服务器的地址
        int sock;//套接字
        char* buff=(char*)malloc(512);
        std::string temp;
        struct DataBody * rxdatabody = new DataBody;
        struct DataBody * txdatabody = new DataBody;
        char msg[512];
       
};



#endif