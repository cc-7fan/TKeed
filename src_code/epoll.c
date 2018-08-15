//
// Latest edit by TeeKee on 2017/7/23.
//

#include "epoll.h"

struct epoll_event* events;

int tk_epoll_create(int flags){         //创建一个epoll的句柄，flag用来告诉内核这个监听的数目一共有多大
    int epoll_fd = epoll_create1(flags);            //创建一个epoll的句柄,这个函数会返回一个新的epoll句柄
    //之后的所有操作将通过这个句柄来进行操作
    if(epoll_fd == -1)
        return -1;
    events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * MAXEVENTS);

    return epoll_fd;
}

// 注册新描述符
int tk_epoll_add(int epoll_fd, int fd, tk_http_request_t* request, int events){     //事件注册函数
    struct epoll_event event;
    event.data.ptr = (void*)request;
    event.events = events;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
        //epoll_ctl();第一个参数是epoll句柄   第二个参数表示动作   第三个参数是需要监听的fd
        //第四个参数表示要监听的什么事
    if(ret == -1)
        return -1;
}

// 修改描述符状态
int tk_epoll_mod(int epoll_fd, int fd, tk_http_request_t* request, int events){
    struct epoll_event event;
    event.data.ptr = (void*)request;
    event.events = events;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1)
        return -1;
}

// 从epoll中删除描述符
int tk_epoll_del(int epoll_fd, int fd, tk_http_request_t* request, int events){
    struct epoll_event event;
    event.data.ptr = (void*)request;
    event.events = events;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
    if(ret == -1)
        return -1;
}

// 返回活跃事件数,等待事件的产生
int tk_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout){
     //第四个参数timeout是 epoll_wait的超时，为0的时候表示马上返回，为-1的时候表示一直等下去，
        //直到有事件范围，为任意正整数的时候表示等这么长的时间,
        //一般如果网络主循环是单独的线程的话，可以用-1来等，这样可以保证一些效率，
        //如果是和主逻辑在同一个线程的话，则可以用0来保证主循环的效率。
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);
    //该函数返回需要处理的事件数目，如返回0表示已超时。
    return ret_count;
}

// 分发处理函数
void tk_handle_events(int epoll_fd, int listen_fd, struct epoll_event* events, 
    int events_num, char* path, tk_threadpool_t* tp){

    for(int i = 0; i < events_num; i++){
        // 获取有事件产生的描述符
        tk_http_request_t* request = (tk_http_request_t*)(events[i].data.ptr);
        int fd = request->fd;

        // 如果有新的连接
        if(fd == listen_fd) {
            accept_connection(listen_fd, epoll_fd, path);       //接收这个连接
        }
        else{
            // 有事件发生的描述符为连接描述符

            // 排除错误事件
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                || (!(events[i].events & EPOLLIN))){
                close(fd);
                continue;
            }

            // 将请求任务加入到线程池中
            int rc = threadpool_add(tp, do_request, events[i].data.ptr);
            // do_request(events[i].data.ptr);
        }
    }
}
