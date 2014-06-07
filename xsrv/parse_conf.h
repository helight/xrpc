//===================================================
//	1 section 和 key大小写不敏感
//	2 每行的最大长度 1024
//====================================================
#ifndef PARSE_CONF_H
#define PARSE_CONF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>

using namespace  std;

#define MAX_LENGTH 1024 //一行信息的最大长度

enum ErrorCode
{
	SUCCEEDED=0,		//正确
	FAILED=-1001,		//错误
	FILE_OPEN_ERROR=-1002,	//文件找不到
	PARAM_ERROR=-1003,	//参数错误
	MEM_NOT_ENOUGH=-1004,	//内存不够
	BUF_NOT_ENOUTH=-1005,	//buffer太小
	SEC_NOT_FOUND=-1006,	//找不到section
	KEY_NOT_FOUND=-1007	//找不到key
};

class CReadIni
{
public:
	CReadIni();
	~CReadIni();

	//将ini文件加载到map中
	int LoadConfigFile(const char* pFileName/*ini文件名称*/);

	//读取字符串
	int GetConfigStr(const char* pSection, const char* pKey, char* pOutBuf, unsigned long dwBufLen);

	//读取整数,返回值 < -1000请判断错误情况
	int GetConfigInt(const char* pSection, const char* pKey);

private:

	int GetSection(char* pBuf, string & strSection); 
	int GetKey(char* pBuf, string & strKey, string & strValue);
	int GetValue(char* pBuf, string & strValue);
	int GetStr(char* &pBuf, string & str,char chEnd);

	int AddToMap(string section, string key, string value);

	char CharToLower(char c);
	string ToLower(const char *pszValue);

private:
	// --- section,key,value --- //
	map< string ,map< string ,string > >  m_mapConfig;

};
#endif //PARSE_CONF_H
