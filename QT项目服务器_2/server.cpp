#include "server.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include "debug_and_log_lr.h"
#include<sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include<string>
#include<queue>
#include<map>
#include<list>
using namespace std;
#include"protocol.h"
#include"tool.h"


server::server(int port)
{
	this->my_error = debug_and_log_lr::get_error_lr();
	this->new_lfd_bind_list(port);
}
void server::wait_message()
{

	
	//保存epoll_wait的返回值
	struct epoll_event events[1024];

	my_error->run_print("服务器开始运行：");

	while (1)
	{
		//epoll监听
		int ret_epollwait = epoll_wait(this->epfd, events, sizeof(events) / sizeof(events[0]), 5000);
		my_error->run_print("epoll解除阻塞了一次：当前的变化的套接字数量为",to_string(ret_epollwait).data());

		//两秒没有连接和活跃套接字处理，就检测一下当前的客服端
		if (ret_epollwait == 0)
		{
			my_error->run_print("空闲时间，开始处理一些套接字：");
			char temp_buf[2];
			temp_buf[0] = 0B00000011;
			temp_buf[1] = 0B00000000;

			int i = 0;
			list<ptr*>::iterator it = this->my_epoll.begin();
			//没有经过4次挥手失效的已关闭套接字、长时间不活跃的套接字，释放
			while (it != this->my_epoll.end())
			{
				++i;
				int current_time = time(NULL);

				my_error->run_print((string("开始处理第“") + string(to_string(i))+string("”个套接字：")).data(), 
					"套接字ID=", to_string((*it)->fd).data());

				if (write((*it)->fd, temp_buf, 2) == -1)
				{
					//这个宏是对方缓冲区满了，不用关闭连接
					if (errno == EAGAIN)
					{
						my_error->run_print("第", to_string(i).data(), "个套接字写缓冲区满了，不做处理");
						it++;
						continue;
					}
					my_error->run_print("第", to_string(i).data(), "个套接字“无法通信”关闭！");
					//下树
					epoll_ctl(epfd, EPOLL_CTL_DEL, (*it)->fd, NULL);
					//关闭套接字
					close((*it)->fd);
					//自己维护一份ptr空指针,先++迭代器，再删除迭代器中的元素
					ptr* temp = (*it);
					it++;
					this->my_epoll.remove(temp);
					//释放ptr空指针占用的空间
					delete (temp);
					
					continue;
				}

				if ((current_time - (*it)->active_time) >= DELETE_CFD)
				{
					my_error->run_print("第", to_string(i).data(), "个套接字一段时间时间没有活跃！");
					if ((*it)->is_close == false)
					{
						if ((current_time - (*it)->active_time) >= FALSE_DELETE_CFD)
						{
							my_error->run_print("第", to_string(i).data(), "个套接字“太长时间没有活跃”关闭！");
							//下树
							epoll_ctl(epfd, EPOLL_CTL_DEL, (*it)->fd, NULL);
							//关闭套接字
							close((*it)->fd);
							//自己维护一份ptr空指针,先++迭代器，再删除迭代器中的元素
							ptr* temp = (*it);
							it++;
							this->my_epoll.remove(temp);
							//释放ptr空指针占用的空间
							delete (temp);
							continue;
						}
						else
						{
							my_error->run_print("第", to_string(i).data(), "个套接字一段时间时间没有活跃，但是处于特殊情况中暂不关闭！");
							it++;
							continue;
						}
					}
					else
					{
						my_error->run_print("第", to_string(i).data(), "个套接字“一段时间时间没有活跃”关闭！");
						//下树
						epoll_ctl(epfd, EPOLL_CTL_DEL, (*it)->fd, NULL);
						//关闭套接字
						close((*it)->fd);
						//自己维护一份ptr空指针,先++迭代器，再删除迭代器中的元素
						ptr* temp = (*it);
						it++;
						this->my_epoll.remove(temp);
						//释放ptr空指针占用的空间
						delete (temp);
						continue;
					}
				}
				else
				{					
					it++;
					continue;
				}
				my_error->run_print("第", to_string(i).data(), "个套接字无法特殊情况！");
			}


		}


		//处理有变化的套接字
		for (int i = 0; i < ret_epollwait; i++)
		{
			//保存需要处理的请求
			message cfd_message;
			//获取到ptr指针
			ptr* temp_ptr = (ptr*)events[i].data.ptr;

			//处理一个套接字的数据
			if ((events[i].events & EPOLLIN))
			{
				//调用读事件处理函数，返回读取到的一条或多条消息
				cfd_message.data = (this->*temp_ptr->read_event)(events[i]);
				//如果是读到了一条或多条消息
				if (cfd_message.data.empty() != true)
				{
					cfd_message.message_ptr = temp_ptr;
					protocol protocol(cfd_message);
				}

			}

			////如果写事件变化，调用套接字的处理方法
			//if (events[i].events == EPOLLOUT)
			//	temp_ptr->writre_event(events[i]);
			////如果错误事件变化，调用套接字的处理方法
			//if (events[i].events == EPOLLERR)
			//	temp_ptr->error_event(events[i]);

		}
	}

}

void server::new_lfd_bind_list(int port)
{
	//创建监听套接字
	this->lfd = socket(PF_INET, SOCK_STREAM, 0);
	if (this->lfd == -1)
		this->my_error->log_error_lr("socket()");

	//绑定
	struct sockaddr_in s_addr;
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->lfd, (struct sockaddr*) & s_addr,
		sizeof(s_addr)) == -1)
		this->my_error->log_error_lr("bind()");

	//监听
	if (listen(this->lfd, 128))
		this->my_error->log_error_lr("listen()");

	//创建epoll树
	if ((this->epfd = epoll_create(1)) == -1)
		this->my_error->log_error_lr("epoll_create()");

	epoll_event temp_event;
	//监视连接的lfd套接字上树
	//ptr* lfd_ptr = (ptr*)malloc(sizeof(ptr));	//创建节点的自定义数据
	ptr* lfd_ptr = new ptr;
	lfd_ptr->read_event = &server::read_event_lfd;	//给节点读事件处理方法
	lfd_ptr->fd = this->lfd;	//套接字
	memset(&temp_event, 0, sizeof(temp_event)); //使用上树节点前清空一下
	temp_event.data.ptr = lfd_ptr;	//自定义数据给上树节点
	temp_event.events = EPOLLIN; //需要监听的事件
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->lfd, &temp_event) == -1)//调用epoll上树函数
		this->my_error->log_error_lr("new_lfd_bind_list() --->> epoll_create()");


}


queue<string> server::read_event_lfd(epoll_event ev)
{
	//使用accept函数受理，处理连接
	int cfd=0;
	struct sockaddr_in temp_caddr;
	int caddr_siz = sizeof(temp_caddr);
continue_accept:
	if ((cfd = accept(this->lfd, (struct sockaddr*) & temp_caddr, (socklen_t*)& caddr_siz)) == -1)
	{
		//被信号打断,重新处理
		if (errno == EINTR)
			goto continue_accept;
		this->my_error->log_error_lr("accept()");
	}


	//新的套接字上树
	epoll_event temp_event;
	memset(&temp_event, 0, sizeof(temp_event));
	//ptr* cfd_ptr = (ptr*)malloc(sizeof(ptr));
	ptr* cfd_ptr = new ptr;
	cfd_ptr->active_time = time(NULL);
	cfd_ptr->read_event = &server::read_event_cfd;
	cfd_ptr->fd = cfd;
	temp_event.data.ptr = cfd_ptr;
	temp_event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &temp_event);

	//自己维护一份ptr空指针
	this->my_epoll.push_back(cfd_ptr);

	// 设置套接字为非阻塞
	// 获取套接字flag属性
	int flag = fcntl(cfd, F_GETFL);
	// 添加非阻塞属性
	flag = flag | O_NONBLOCK;
	// 将新的flag属性设置给cfd
	fcntl(cfd, F_SETFL, flag);

	//连接建立后打印客服端IP
	//char cip[100] = { 0 };
	//if (inet_ntop(AF_INET, &temp_caddr.sin_addr.s_addr, cip, sizeof(cip)) == NULL)
	//	this->my_error->log_error_lr("inet_ntop()");
	char cip[128] = { 0 };
	my_error->run_print("新的连接到来", to_string(cfd_ptr->fd).data(), inet_ntop(AF_INET, &temp_caddr.sin_addr.s_addr, cip, sizeof(cip)));


	//this->lt.tm_year = temp_lt->tm_year;
	//this->lt.tm_mon = temp_lt->tm_mon;
	//this->lt.tm_mday = temp_lt->tm_mday;
	//this->lt.tm_hour = temp_lt->tm_hour;
	//this->lt.tm_min = temp_lt->tm_min;
	//this->lt.tm_sec = temp_lt->tm_sec;
	//
	//string sql = "INSERT INTO temp_ip (ip_date,ip_info,ip_info_aes)VALUES(";
	//	sql += "\" ";
	//	2014 - 10 - 10:10 : 10 : 10, asf, asd);

	//年有点问题有时间改一下
	char ip_buf[128] = { 0 };
	my_error->up_time_lr();
	sprintf(ip_buf, " INSERT INTO temp_ip(ip_date, ip_info, ip_info_aes)VALUES(\'%d-%d-%d:%d:%d:%d\',\"%s\", \"%s\");",
		my_error->lt.tm_yday, my_error->lt.tm_mon,
		my_error->lt.tm_mday, my_error->lt.tm_hour,
		my_error->lt.tm_min, my_error->lt.tm_sec,
		cip, "加密后的IP暂时留空");
	my_sql mysql;
	mysql_free_result(mysql.sql_exec(ip_buf));


	return queue<string>();
}


queue<string> server::read_event_cfd(struct epoll_event ev)
{
	char buf[1024] = { 0 };
	queue<string> ret_queue;

	ptr* cfd_ptr = (ptr*)ev.data.ptr;
	string& data = cfd_ptr->data;

	while (1)
	{

		memset(buf, 0, sizeof(buf));

		int ret_read = read(cfd_ptr->fd, buf, sizeof(buf));


		//客服端断开连接
		if (ret_read == 0)
		{
			my_error->run_print("--------正常关闭了一个套接字", to_string(cfd_ptr->fd).data());

			//下树
			epoll_ctl(epfd, EPOLL_CTL_DEL, cfd_ptr->fd, NULL);
			//自己维护一份ptr空指针
			this->my_epoll.remove(cfd_ptr);
			//关闭套接字
			close(cfd_ptr->fd);
			//释放ptr空指针占用的空间
			delete cfd_ptr;

			break;
		}
		//本次数据读取完毕
		else if (ret_read == -1)
		{
			//epoll节点使用ET模式  读取非阻塞套接字  套接字缓冲区中没有数据时返回这个宏  
			if (errno == EAGAIN)
			{
				my_error->run_print("读取到了一条正常请求", to_string(cfd_ptr->fd).data());
				cfd_ptr->active_time = time(NULL);
				break;
			}

			//宏104，收到了数据但是对方已关闭，会走4次挥手，无需在这关闭
			this->my_error->log_error_lr("read_cfd() --->> read()");

		}
		//读取到了数据
		else if (ret_read >= 1)
		{
			for (int i = 0; i < ret_read; i++)
			{
				if (buf[i] == '\0')
				{
					ret_queue.push(data);
					//是否相当于初始化
					data = string();
				}
				else
				{
					data += buf[i];
				}
			}
		}

	}


	return ret_queue;

}

ptr::~ptr()
{
	if (cfd_temp_key != NULL)
	{
		
		delete cfd_temp_key;
		cfd_temp_key = NULL;
	}
}
