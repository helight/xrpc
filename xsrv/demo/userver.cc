#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "server/xudpserver.h"
#include "server/config.h"
// #include "xsrv/dbconn.h"

class TestRpc
{
public:
	TestRpc(json::rpc::Handler& handler)
	{
		handler.AddMethod(new json::rpc::RpcMethod<TestRpc>(*this,
					&TestRpc::Print, "Print"));
		handler.AddMethod(new json::rpc::RpcMethod<TestRpc>(*this,
					&TestRpc::show_project, "show_project"));
	}

	bool Print(const json::Value& msg, json::Value& response, void *data)
	{
		struct client *cli= (struct client*)data;
		response["jsonrpc"] = "2.0";
		response["id"] = msg["id"];
		response["result"] = "success";
		response["time"] = (char)time(NULL);
		response["xip"] = inet_ntoa(cli->ipaddr);

		return true;
	}
	bool show_project(const json::Value& msg, json::Value& response, void *data)
	{
		const json::Value& project_id = msg["project_id"];
		response["jsonrpc"] = "2.0";
		response["id"] = project_id;
		response["result"] = "success";

		return true;
	}
};

int main(int argc,char **argv){
	json::rpc::Handler handler;
	TestRpc rpc1(handler);

	bool b = XAppConfig::GetInstance()->LoadConf("tconf.ini");
	if (!b) {
		printf("load testconf.ini fail\n");
		return false;
	}
	printf("localIp=%s\nmyRole=%s\nmaster_db_ip=%s\nmaster_db_port=%d\n",
			XAppConfig::GetInstance()->GetMyIdentification()->localIp,
			XAppConfig::GetInstance()->GetMyIdentification()->myRole,
			XAppConfig::GetInstance()->GetMyIdentification()->master_db_ip,
			XAppConfig::GetInstance()->GetMyIdentification()->master_db_port
		  );

	//dbconn(char* szHost,char* szDB,char* szUser,char* szPass,uint szPort)

	xudpserver server(std::string("127.0.0.1"), 8080);

	if (server.Start(handler)) {
		printf("init server\n");
	} else {
		printf("error");
	}

	return 1;
}
