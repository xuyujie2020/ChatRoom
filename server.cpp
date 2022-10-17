
#include "server.h"

 
#define NFDS 100//fds数组的大小

/***
 * 绑定服务器地址
 */

Server::Server()
{
    //第一步初始化端口
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(6000);
	server_address.sin_addr.s_addr = inet_addr("172.16.47.198");
    //fds = (struct pollfd*)malloc(sizeof(struct pollfd) * NFDS);
    fds = new pollfd[NFDS];
    listenfd =0;
	fdcount=0;

	memset(trdata,0,sizeof(DataBody));

}
Server::~Server()
{
	onclient.clear();
    delete fds;
	delete trdata;
}

void Server::Init()
{
    std::cout<<"\t初始化TCP服务端\t"<<std::endl;

    //第二步socket创建套接字
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(-1 != listenfd);

    std::cout<<"\t创建套接字成功\t"<<std::endl;

    //第三步bind绑定端口
    res =  bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address));
	assert(-1 != res);

    std::cout<<"\tbind绑定端口成功\t"<<std::endl;
 
    //第四步设置监听
	res = listen(listenfd, 5);
    assert(-1 != res);

    std::cout<<"\t设置监听成功\t"<<std::endl;

	for(int i = 0; i < NFDS; ++i)
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		fds[i].revents = 0;
	}
    std::cout<<"\t初始化客户端描述符数组成功\t"<< std::endl ;

    std::cout<<"\t初始化TCP服务端完成\t"<<std::endl;

}

void Server::DeleteFd(int fd)
{
	
	for(int i = 0; i < NFDS; ++i)
	{
		if(fds[i].fd == fd)
		{
			fds[i].fd = -1;
			fds[i].events = 0; 
			std::cout.flush();//清空缓存区
			std::cout<<"客户端："<<fd<<"\t断开"<<std::endl;
			onclient.remove(fd);//删除fd
			
			break;
		}
	}
} 


void Server::InsertFd(int fd, int flag)
{
    
	for(int i = 0; i < NFDS; ++i)
	{
		if(fds[i].fd == -1)
		{
			fds[i].fd = fd;
			fds[i].events |= POLLIN; 
			if(flag)
			{
				fds[i].events |= POLLRDHUP;
			}
			break;
		}
	}
}

void Server::GetClient(int fd)
{
    struct sockaddr_in cli;
	socklen_t len = sizeof(cli); 
	int c = accept(fd, (struct sockaddr*)&cli, &len);
	assert(c != -1);
	printf("one client link success\n");
	InsertFd(c, 1);
	onclient.push_back(c);
	std::cout<<"客户端\t["<<c<<"]\t连接成功!"<<std::endl;
}

void Server::PushClient(int fd)
{
	
    close(fd);
	DeleteFd(fd);
}


void Server::SendMsgAll()
{
	for(auto i : onclient)
	{
		
		DataProcess::SendPackage(i,trdata);
	}
}
/**
 * 向指定客户端发送消息
*/
void Server::SendMsgClient()
{

}

void Server::DealClientData(int fd)
{
	trdata = DataProcess::RecvPackage(fd);
	trdata->datahead.fd = fd;
	if(trdata->datahead.type == Type::File)
	{
		SendMsgClient();
	}
	else if(trdata->datahead.type == Type::Text)
	{
		SendMsgAll();
	}

}

void Server::GetClientOnline()
{
	for(auto i : onclient)
	{
		std::cout<<"用户\t"<<i<<"\t在线"<<std::endl;
	}
}

void Server::DealRreadFd(int fd)
{
    
	for(int i = 0; i < NFDS; ++i)
	{
		if(fds[i].fd == -1)
		{
			continue;
		}
 
		int fd = fds[i].fd;
		if(fd == listenfd && fds[i].revents & POLLIN)
		{	
			//获取连接
			GetClient(fd);
			
			std::cout<<"目前已连接\t"<<onclient.size()<<"\t个客户端"<<std::endl;
			GetClientOnline();
			
		}
		else if(fds[i].revents & POLLRDHUP)
		{
			PushClient(fd);
			//断开连接
		}
		else if(fds[i].revents & POLLIN)
		{
			DealClientData(fd);
			//处理客户端数据
		}
	}
}
void Server::Start()
{

    Init();

    InsertFd(listenfd,0);

    while(1)
	{
		int n = poll(fds, NFDS, -1);
		if(n <= 0)
		{
			printf("poll error\n");
			continue;
		}
		DealRreadFd(listenfd);
		//处理就绪的文件描述符
	}

}


