#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "xtcpclient.h"

xtcpclient::xtcpclient(const std::string& address, uint16_t port)
{
	m_sock = -1;
	m_address = address;
	m_port = port;
}

xtcpclient::~xtcpclient()
{
	if (m_sock != -1) {
		close();
	}
}

int xtcpclient::connect()
{
	int len;
	int result;
	int sockfd;
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(m_address.c_str());
	address.sin_port = htons(m_port);
	len = sizeof(address);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	result = ::connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1) {
		perror("oops: client1");
		::close(sockfd);
		return 1;
	}
	m_sock = sockfd;
	return 0;
}

void xtcpclient::close()
{
	::close(m_sock);
	m_sock = -1;
}

ssize_t xtcpclient::recv(std::string& data)
{
	char buf[1500];
	ssize_t nb = -1;

	if ((nb = ::recv(m_sock, buf, sizeof(buf), 0)) == -1) {
		perror("Error while receiving\r\n");
		return -1;
	}

	data = std::string(buf, nb);
	return nb;
}

ssize_t xtcpclient::send(const std::string& data)
{
	std::string rep = data;

	return ::send(m_sock, rep.c_str(), rep.length(), 0);
}
