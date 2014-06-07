// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-07
// Description:
//

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include "ev.h"

#define PORT 9999
#define BUFFER_SIZE 1024


void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);


void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

int main()
{
    struct ev_loop *loop = ev_default_loop(0);
    int sd;
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);
    struct ev_io socket_accept;

    // 创建socket的写法,这里简单处理,用INADDR_ANY ,匹配任何客户端请求.这里写法都一样,没什么特别的,直接copy都可以用
    if( (sd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
    {
        std::cout << "socket error";
        return -1;
    }
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
        std::cout << "bind error"  << std::endl;
    }
    if (listen(sd, 2) < 0)
    {
        std::cout << "listen error"  << std::endl;
        return -1;
    }



    // 初始化,这里监听了io事件,写法参考官方文档的
    ev_io_init(&socket_accept, accept_cb, sd, EV_READ);
    ev_io_start(loop, &socket_accept);


    while (1)
    {
        ev_loop(loop, 0);
    }

    return 0;
}



//accept事件 的回调块
void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sd;

    //分派客户端的ev io结构
    struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

    //libev的错误处理
    if(EV_ERROR & revents)
    {
        std::cout << "error event in accept"  << std::endl;
        return;
    }

    //accept,普通写法
    client_sd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sd < 0)
    {
         std::cout << "accept error"  << std::endl;
        return;
    }

     std::cout << "someone connected.\n"  << std::endl;

    //开始监听读事件了,有客户端信息就会被监听到
    ev_io_init(w_client, read_cb, client_sd, EV_READ);
    ev_io_start(loop, w_client);
}

//read 数据事件的回调快
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents){
    char buffer[BUFFER_SIZE];
    ssize_t read;

    if(EV_ERROR & revents)
    {
         std::cout << "error event in read"  << std::endl;
        return;
    }

    //recv普通socket写法
    read = recv(watcher->fd, buffer, BUFFER_SIZE, 0);

    if(read < 0)
    {
         std::cout << "read error"  << std::endl;
        return;
    }

    //断开链接的处理,停掉evnet就可以,同时记得释放客户端的结构体!
    if(read == 0)
    {
         std::cout << "someone disconnected."  << std::endl;
        ev_io_stop(loop,watcher);
        free(watcher);
        return;
    }
    else
    {
         std::cout << "get the message:" << buffer << std::endl;
    }

    //原信息返回,也可以自己写信息,都一样.最后记得置零
    send(watcher->fd, buffer, read, 0);
    bzero(buffer, read);
}

