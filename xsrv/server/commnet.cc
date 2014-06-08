// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//

#include <iostream>
#include "commnet.h"
json::rpc::Handler *xhandler;

static void Recv(struct ev_loop *loop, struct ev_io *w, int revents);
static void Send(struct ev_loop *loop, struct ev_io *w, int revents);
static void udp_send(struct ev_loop *loop, struct ev_io *w, int revents);
bool check(std::string &msg, json::Value &recv, json::Value &response);
std::string GetString(json::Value value);

bool check(std::string &msg, json::Value &recv, json::Value &response)
{
	bool parsing = false;
	json::Value error;
	json::Reader m_reader;
	parsing = m_reader.parse(msg, recv);

	if (!parsing) {
		/* request or batched call is not in JSON format */
		response["id"] = json::Value::null;
		response["jsonrpc"] = "2.0";

		error["code"] = "error";
		error["message"] = "Parse error.";
		response["error"] = error;
		return false;
	}
	return true;
}

std::string GetString(json::Value value)
{
	json::FastWriter m_writer;
	return m_writer.write(value);
}

int setnonblock(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return flags;
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		return -1;

	return 0;
}

void Accept(struct ev_loop *loop, struct ev_io *w, int revents)
{
	int client_fd;
	struct client *client;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	client_fd = accept(w->fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1) {
		return ;
	}

	client = (struct client *)calloc(1,sizeof(*client));
	client->fd = client_fd;
	client->ipaddr = client_addr.sin_addr;
	client->handler = xhandler;

	if (setnonblock(client->fd) < 0) {
		err(1, "failed to set client socket to non-blocking");
	}

	ev_io_init(&client->ev_read, Recv, client->fd, EV_READ);
	ev_io_start(loop, &client->ev_read);
}

static void Recv(struct ev_loop *loop, struct ev_io *w, int revents)
{
	struct client *cli = ((struct client*) (((char*)w) - offsetof(struct client, ev_read)));
	int r = 0;

	if (revents & EV_READ) {
		r = read(cli->fd, cli->rbuff, 1024);
	}

	ev_io_stop(EV_A_ w);
	ev_io_init(&cli->ev_write, Send, cli->fd, EV_WRITE);
	ev_io_start(loop, &cli->ev_write);
}

static void Send(struct ev_loop *loop, struct ev_io *w, int revents)
{
    std::string response;
	struct client *cli= ((struct client*) (((char*)w) - offsetof(struct client,ev_write)));

    std::string rbuff = cli->rbuff;
	if (check(rbuff, cli->recv, cli->response)) {
		cli->response["id"] = cli->fd;
		cli->response["jsonrpc"] = "2.0";
		cli->response["code"] = "OK";
	}
	//	cli->response["ip"] = inet_ntoa(cli->ipaddr);
	cli->handler->Process(rbuff, cli->response, (void *)cli);
	response = GetString(cli->response);
	if (revents & EV_WRITE) {
		write(cli->fd, response.c_str(), strlen(response.c_str()));
		ev_io_stop(EV_A_ w);
	}

	close(cli->fd);
	free(cli);
}

void udp_recv(struct ev_loop *loop, struct ev_io *w, int revents)
{
	int r = 0;
	struct client *client;
	socklen_t client_len = sizeof(struct sockaddr_in);

	if (w->fd == -1) {
		return ;
	}
	client = (struct client *)calloc(1,sizeof(*client));
	client->fd = w->fd;
	client->handler = xhandler;

	if (revents & EV_READ) {
		r = recvfrom(client->fd, client->rbuff, 1024, 0,
				(struct sockaddr *)&(client->client_addr), &client_len);
		client->ipaddr = client->client_addr.sin_addr;
		//r = read(client->fd, client->rbuff, 1024);
	}
	std::cout << "rbuff: " << client->rbuff << std::endl;
	ev_io_init(&client->ev_write, udp_send, client->fd, EV_WRITE);
	ev_io_start(loop, &client->ev_write);
}

static void udp_send(struct ev_loop *loop, struct ev_io *w, int revents)
{
    std::string response;
	struct client *cli= ((struct client*) (((char*)w) - offsetof(struct client,ev_write)));

    std::string rbuff = cli->rbuff;
	if (check(rbuff, cli->recv, cli->response)) {
		cli->response["id"] = cli->fd;
		cli->response["jsonrpc"] = "2.0";
		cli->response["code"] = "OK";
	}
	cli->response["ip"] = inet_ntoa(cli->ipaddr);
	cli->handler->Process(rbuff, cli->response, (void *)cli);
	response = GetString(cli->response);
	std::cout << "response: " << response.c_str() << std::endl;
	if (revents & EV_WRITE) {
		sendto(cli->fd, response.c_str(), strlen(response.c_str()), 0,
				(struct sockaddr *)&cli->client_addr, sizeof(struct sockaddr_in));
		//write(cli->fd, response.c_str(), strlen(response.c_str()));
		ev_io_stop(EV_A_ w);
	}

	//close(cli->fd);
	free(cli);
}

static void sighandler(int signum)
{
        if (signum == SIGCHLD) {
                /* clean up child processes */
                while(waitpid(-1,NULL,WNOHANG) > 0);
        } else {
                while(waitpid(-1,NULL,WNOHANG) > 0);

                close(0);
                close(1);
                //exit(0);
        }
}

void InitDaemon()
{
	rlimit rlim,rlim_new;
	if (getrlimit(RLIMIT_NOFILE, &rlim)==0) {
		rlim_new.rlim_cur = rlim_new.rlim_max = 100000;
		if (setrlimit(RLIMIT_NOFILE, &rlim_new)!=0) {
			//Warn("%s","[Main]:Setrlimit file Fail, use old rlimit!\n");
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			setrlimit(RLIMIT_NOFILE, &rlim_new);
		}
	}

	if (getrlimit(RLIMIT_CORE, &rlim)==0) {
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new)!=0) {
			//Warn("%s","[Main]:Setrlimit core Fail, use old rlimit!\n");
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			setrlimit(RLIMIT_CORE, &rlim_new);
		}
	}

	signal(SIGPIPE, SIG_IGN);
	pid_t pid;

	if ((pid = fork() ) != 0) {
		exit( 0);
	}

	setsid();
	signal( SIGINT,  SIG_IGN);
	signal( SIGHUP,  SIG_IGN);
	signal( SIGQUIT, SIG_IGN);
	signal( SIGPIPE, SIG_IGN);
	signal( SIGTTOU, SIG_IGN);
	signal( SIGTTIN, SIG_IGN);
	signal( SIGCHLD, SIG_IGN);
	signal( SIGTERM, SIG_IGN);

	struct sigaction sig;
	sig.sa_handler = sighandler;//SIG_IGN;
	sig.sa_flags = 0;
	sigemptyset( &sig.sa_mask);
	sigaction( SIGHUP,&sig,NULL);
	if ((pid = fork() ) != 0) {
		exit(0);
	}
	umask(0);
	setpgrp();
}
