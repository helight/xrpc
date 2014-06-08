// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description: net api
//

#ifndef XCOMMNET_H
#define XCOMMNET_H
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <string>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include "jsonrpc/jsonrpc.h"
#include "jsonrpc/json.h"
#include "ev.h"

struct client {
	int fd;
	ev_io ev_write;
	ev_io ev_read;
	struct sockaddr_in client_addr;
	struct in_addr	ipaddr;			/*client ip*/
	char rbuff[1024];
	json::Value response;
	json::Value recv;
	json::rpc::Handler *handler;
};

void InitDaemon();
int setnonblock(int fd);
void SigHandler(int signum);

/**
 * \brief Accept a new client socket.
 */
void Accept(struct ev_loop *loop, struct ev_io *w, int revents);
void udp_recv(struct ev_loop *loop, struct ev_io *w, int revents);

#endif /* XCOMMNET_H */

