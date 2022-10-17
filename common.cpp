#include "common.h"


namespace DataProcess
{

    void SendPackage(int fd,DataBody* databody)
    {
        if(!send(fd,databody,databody->datahead.data_size,0))
        {
            std::cerr<<"发送消息失败"<<std::endl;
        }
    }

    DataBody* RecvPackage(int fd)
    {
        DataBody* databody = new DataBody();
        int datatotlesize=0;//接收到的数据总大小
        int dataheadsize=0;//接收到的数据包大小
        int datacontantsize=0;//接收到的数据内容总大小
        int onceget=0;
        unsigned char recv_buffer[10000];
        int datasize =0;

        //使用循环方法，接收数据包头//while防止消息接收不完整
        while(datatotlesize != sizeof(DataHead))
        {
            onceget = recv(fd,&databody->datahead,sizeof(DataHead) - datatotlesize,0);
            if(onceget == 0)
            {
                std::cerr<<"获取数据包头错误"<<std::endl;
            }
            datatotlesize += onceget ;
        }

        //接收数据内容
        while(datatotlesize != databody->datahead.data_size)
        {
            onceget = recv(fd,&databody->content + datatotlesize - sizeof(DataHead),databody->datahead.data_size - datatotlesize,0);
            if(onceget == 0)
            {
                std::cerr<<"获取数据内容错误"<<std::endl;
            }
            datatotlesize += onceget ;
        }

        return databody;
    }
} // namespace DataProces
