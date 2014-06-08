#ifndef _CONFIG_H
#define _CONFIG_H
#include "parse_conf.h"
#include <vector>
#include <set>
#include <string>

using namespace std;

struct MyIdentification
{
	char localIp[20];
	char myRole[20];
	char master_db_ip[20];
	char master_db_user[30];
	char master_db_pass[30];
	char master_db_db[30];
	unsigned int master_db_port;
};


class XAppConfig
{
public:

	// 获取单实例
	inline static XAppConfig* GetInstance()
	{
		if(m_pAppConf == NULL)
			m_pAppConf = new XAppConfig;
		return m_pAppConf;
	}

	// 释放单实例
	static void FreeInstance()
	{
		if (m_pAppConf != NULL) {
			delete m_pAppConf;
			m_pAppConf = NULL;
		}
	}

	//加载配置文件
	bool LoadConf(const char* pConf);
	MyIdentification *GetMyIdentification(){ return &myIdentification;}

private:
	XAppConfig(){}
	~XAppConfig(){}

private:
	static XAppConfig* m_pAppConf;

	CReadIni ini;
	MyIdentification myIdentification;

};


#endif
