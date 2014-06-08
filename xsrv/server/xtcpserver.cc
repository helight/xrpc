// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description: tcp server
//

#include "xtcpserver.h"
extern json::rpc::Handler *xhandler;

xtcpserver::xtcpserver(const std::string& address, uint16_t port)
{
	m_sock = -1;
	m_address = address;
	m_port = port;
}

xtcpserver::~xtcpserver()
{
	if (m_sock != -1) {
		Close();
	}
}

bool xtcpserver::Init()
{
	if (m_sock > 0)
		return true;
	int sock;
	struct sockaddr_in listen_addr;
	int reuseaddr_on = 1;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		err(1, "listen failed");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
				sizeof(reuseaddr_on)) == -1)
		err(1, "setsockopt failed");
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = inet_addr(m_address.c_str());
	listen_addr.sin_port = htons(m_port);

	if (bind(sock, (struct sockaddr *)&listen_addr,
				sizeof(listen_addr)) < 0)
		err(1, "bind failed");

	if (sock < -1) {
		return false;
	}

	if (listen(sock, 5) < -1) {
		return false;
	}
	if (setnonblock(sock) < 0)
		err(1, "failed to set server socket to non-blocking init");
	m_sock = sock;

	return (m_sock != -1) ? true : false;
}

bool xtcpserver::Start(json::rpc::Handler &handler)
{
	ev_io ev_accept;
	struct ev_loop *loop;

	if (!Init())
		err(1, "init failed");
#ifndef DEBUG
	InitDaemon();
#endif
	xhandler = &handler;

	loop = ev_default_loop (0);
	ev_io_init(&ev_accept, Accept, m_sock, EV_READ);
	ev_io_start(loop,&ev_accept);
	ev_loop (loop, 0);
}

int xtcpserver::GetSocket() const
{
	return m_sock;
}

std::string xtcpserver::GetAddress() const
{
	return m_address;
}

uint16_t xtcpserver::GetPort() const
{
	return m_port;
}

void xtcpserver::Close()
{
	::close(m_sock);
	m_sock = -1;
}

