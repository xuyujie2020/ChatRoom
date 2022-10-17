#ifndef COMMON_H
#define COMMON_H

#include<arpa/inet.h>//selcet
#include <unistd.h>//uni std
#include<string.h>

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>

#include <poll.h>
#include <assert.h>
#include <list>
#include <vector>
#include <stdio.h>//itoa
#include <stdlib.h>

#include <fstream>//文件io

#define DATA_PACKEAGE 1024*1024 //1M缓存

enum class Type
{
    File,
    Text
};

struct DataHead
{
	Type type;//消息类型
    int data_size;//数据包总大小，包括消息和消息头
    int fd;//从该套接字获取的消息
    int recvfd;//目标文件
    std::string filename;
    
};

/**
 * 每次发送和接收一个该结构体大小的消息
*/
struct DataBody
{
    struct DataHead datahead;
    char content[DATA_PACKEAGE];//消息内容
};

namespace DataProcess
{
    /**
     * 发送数据包
    */
    void SendPackage(int fd,DataBody* databody);

    /**
     * 接收数据包
    */
    DataBody* RecvPackage(int fd);
}


 

#endif