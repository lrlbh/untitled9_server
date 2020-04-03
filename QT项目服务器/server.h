#include<iostream>
#include<queue>
#include<map>
#include<list>
class server;



#define  DELETE_CFD 10	//客服端多少时间不活跃会被断开

typedef struct 
{
	//第一个成员必须是fd，epoll_wait函数才能正确赋值 
	int fd;
	//读事件处理函数
	std::queue<std::string>(server::* read_event)(struct epoll_event);

	//处理粘包
	std::string data;

	//记录访问时间
	time_t active_time;

	//注册时绑定套接字的临时秘钥
	void* cfd_temp_key;

	////写事件处理函数
	//void (*writre_event)(struct epoll_event );
	////错误事件处理函数
	//void (*error_event)(struct epoll_event);
	//void* ev_free;


}ptr;



class debug_and_log_lr;


//处理读事件epool对象，ET高性能模式
class server
{
public:
	//构造，开始处理连接
	server(int port =8000);
	//功能：获取到 一个或者多个客服端的 一条或者多条消息 就返回
	std::map<int, std::queue<std::string>> wait_message();

private:
	//监听套接字
	int lfd = -1;
	//打印错误对象
	debug_and_log_lr* my_error =NULL;
	//epoll树的根节点
	int epfd =-1;
	//自己维护epoll节点中联合中的ptr空指针，用于关闭没有4次挥手就断开的套接字，和不活跃的连接
	std::list<ptr*> my_epoll;

	//创建通信套接字，绑定，打开连接队列，lfd上树监听
	void new_lfd_bind_list(int port);
	//通信套接字读事件处理函数
	std::queue<std::string> read_event_cfd(struct epoll_event ev);
	//监听套接字的读事件处理函数
	std::queue<std::string> read_event_lfd(struct epoll_event ev);


	

};
