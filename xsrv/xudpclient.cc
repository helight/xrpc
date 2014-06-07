#include <stdio.h> 
#include <unistd.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <json/json.h>
#include <xudpclient.h>

xudpclient::xudpclient(const std::string& address, uint16_t port)
{
	m_sock = -1;
	m_address = address;
	m_port = port;
	m_sockaddrlen = 0;
}

xudpclient::~xudpclient()
{
	if (m_sock != -1) { 
		close();
	}
}

int xudpclient::connect()
{
	int len;
	int result;    
	int sockfd;
	//struct sockaddr_in address;
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = inet_addr(m_address.c_str());
	m_sockaddr.sin_port = htons(m_port);
	len = sizeof(m_sockaddr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("oops: client1");
		::close(sockfd);
		return 1;
	}
	m_sockaddrlen = sizeof(m_sockaddr);
	//memcpy((void *)m_sockaddr, (void *)address, m_sockaddrlen);
	if (::connect(sockfd, (struct sockaddr *)&m_sockaddr, m_sockaddrlen) == -1) {
		perror("connect error");
		return -1;
	}
	//m_sockaddrlen = sizeof(address);
	m_sock = sockfd;
	return 0;
}

void xudpclient::close()
{
	::close(m_sock);
	m_sock = -1;
}

ssize_t xudpclient::recv(std::string& data)
{
	char buf[1500];
	ssize_t nb = -1;

	if (m_sock > 0) {
		if((nb = ::recvfrom(m_sock, buf, sizeof(buf), 0, NULL, NULL)) == -1){
			perror("Error while receiving\r\n");
			return -1;
		}
		data = std::string(buf, nb);
	} 

	return nb;
}

ssize_t xudpclient::send(const std::string& data)
{
	std::string rep = data;
	if(m_sock > 0)
		return ::sendto(m_sock, rep.c_str(), rep.length(), 0, (struct sockaddr*)&m_sockaddr, m_sockaddrlen);
	else 
		return -1;
}
