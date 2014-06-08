// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description: udp server
//

#ifndef XUDPSERVER_H
#define XUDPSERVER_H
#include <cstdio>
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
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <jsonrpc/jsonrpc.h>
#include <jsonrpc/json.h>
#include <ev.h>
#include "commnet.h"

/**
 * \class xudpserver
 * \brief Abstract JSON-RPC xudpserver.
 */
class xudpserver
{
public:
	/**
	 * \brief Constructor.
	 * \param address network address or FQDN to bind
	 * \param port local port to bind
	 */
	xudpserver(const std::string& address, uint16_t port);

	/**
	 * \brief Destructor.
	 */
	~xudpserver();

	bool Init();

	bool Start(json::rpc::Handler &handler);

	/**
	 * \brief Close listen socket and all client sockets.
	 */
	void Close();

	/**
	 * \brief Get socket descriptor.
	 * \return socket descriptor.
	 */
	int GetSocket() const;

	/**
	 * \brief Get the address.
	 * \return address or FQDN
	 */
	std::string GetAddress() const;

	/**
	 * \brief Get the port.
	 * \return local port
	 */
	uint16_t GetPort() const;

protected:
	/**
	 * \brief Socket descriptor.
	 */
	int m_sock;

	/**
	 * \brief JSON-RPC handler.
	 */
	//       Handler m_jsonHandler;

private:
	/**
	 * \brief Network address or FQDN.
	 */
	std::string m_address;

	/**
	 * \brief Local port.
	 */
	uint16_t m_port;

	struct client client;

	/**
	 * \brief List of client sockets.
	 */
	std::list<int> m_clients;

};

#endif /* XUDPSERVER_H */

