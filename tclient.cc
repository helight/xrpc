#include <stdio.h> 
#include <unistd.h>
#include <string.h> 
#include <xtcpclient.h>

int main(int argc, char *argv[])
{
	int len;
	string recvdata;
	string strreq="{\"id\":10,\"jsonrpc\":\"2.0\",\"method\":\"Print\"}";

	xtcpclient client(std::string("127.0.0.1"), 8086);

	for (int i = 0; i < 10000; i++) {
		client.connect();
		client.send(strreq);
		len = client.recv(recvdata);
		printf("%s\r\n", recvdata.c_str());
		client.close();
	}

	return 0;
}
