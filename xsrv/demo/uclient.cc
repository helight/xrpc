// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client/xudpclient.h"

int main(int argc, char *argv[])
{
	int len = 0;
    std::string recvdata;
	std::string strreq="{\"id\":10,\"jsonrpc\":\"2.0\",\"method\":\"Print\"}";

	xudpclient client(std::string("127.0.0.1"), 8080);

	for (int i = 0; i < 10; i++) {
		client.connect();
		client.send(strreq);
		len = client.recv(recvdata);
		printf("msg:%s  len: %d\r\n", recvdata.c_str(), len);
		client.close();
	}

	return 0;
}
