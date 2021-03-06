// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//

#ifndef XUDPCLIENT_H
#define XUDPCLIENT_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jsonrpc/json.h>

/**
 * \class xtcpclient
 * \brief Abstract JSON-RPC xtcpclient.
 */
class xudpclient
{
public:
	/**
	 * \brief Constructor.
	 * \param address remote network address or FQDN to contact
	 * \param port remote local port to contact
	 */
	xudpclient(const std::string& address, uint16_t port);

	/**
	 * \brief Destructor.
	 */
	~xudpclient();
	int connect();

	/**
	 * \brief Receive data from the network.
	 * \param data if data is received it will put in this reference
	 * \return number of bytes received or -1 if error
	 * \note This method will blocked until data comes.
	 */
	ssize_t recv(std::string& data);
	/**
	 * \brief Send data.
	 * \param data data to send
	 * \return number of bytes sent or -1 if error
	 */
	ssize_t send(const std::string& data);

	void close();

protected:
	int m_sock;

private:
	std::string m_address;
	struct sockaddr_in m_sockaddr;
	socklen_t m_sockaddrlen;

	uint16_t m_port;
};
#endif /* XUDPSERVER_H */
