
#include "client.h"

Client::Client()
{
    res=0;
    memset(&server_address, 0, sizeof(server_address));
	//bzero(&sa, sizeof(sa));
	server_address.sin_family = AF_INET;// 指定协议族
	server_address.sin_port =  htons(6000);//指定端口号
	server_address.sin_addr.s_addr = inet_addr("120.78.130.120"); //指定IP地址

    memset(txdatabody,0,sizeof(DataBody));
    memset(rxdatabody,0,sizeof(DataBody));


}
Client::~Client()
{
    free(buff);
    delete rxdatabody;
    delete txdatabody;
}

void Client::Init()
{
    //1.创建一个流式套接字
	sock = socket(AF_INET, //指定协议族
					 SOCK_STREAM, //指定套接字类型
					 0 //指定应用层协议
				);
	if (sock == -1)
	{
		perror("create socket error");
		return;
	}

	res = connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));
	if (res == -1)
	{
		perror("connect error");
		return;
	}
}

void Client::RecvThread()
{
   
    while(1)
    {   
        rxdatabody = DataProcess::RecvPackage(sock);
        if(rxdatabody->datahead.type==Type::File)
        {
            strcpy(buff,"文件");
        }
        else if(rxdatabody->datahead.type==Type::Text)
        {	
            std::cout<<rxdatabody->datahead.fd<<"\t>>>\t"<<rxdatabody->content<<std::endl;
        }
    }

}

void Client::SendFile(std::string temp)
{
    strcpy(buff,"##发送文件");
}

//对消息进行封装
void Client::EnCode()
{
    //## + 用户 + 路径 表示发送文件//封装文件包
    if(temp[0]=='#'&& temp[1]=='#')
    {
        txdatabody->datahead.type == Type::File;
    }
    else//否则发送普通文字//封装数据包
    {
        memset(txdatabody,0,sizeof(DataBody));

        //初始化txdatabody
        txdatabody->datahead.type=Type::Text;
        txdatabody->datahead.data_size = temp.size() + sizeof(DataHead);
        strcpy(txdatabody->content,temp.c_str());//文字消息使用strcpy，结构体消息使用memcpy
    }
}

void Client::SendData()
{
    EnCode();//编码后得到数据包
    DataProcess::SendPackage(sock,txdatabody);
}

void Client::Start()
{
    //连接服务端
    Init();
    
    //创建线程表示接收消息
     std::thread tid(&Client::RecvThread,this);
    //等待发送消息
    while(1)
    {
        getline(std::cin,temp);
        if(temp.size()>DATA_PACKEAGE)
        {
            std::cout<<"输入消息过长"<<std::endl;
            continue;
        }
        SendData();
    }
}


