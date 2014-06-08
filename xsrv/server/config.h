// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#ifndef _CONFIG_H
#define _CONFIG_H
#include <set>
#include <string>
#include <vector>
#include "parse_conf.h"

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
