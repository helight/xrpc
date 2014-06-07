#include "mysql/mysql.h"
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

#ifndef ulong
#define ulong unsigned long 
#endif

#ifndef uint 
#define uint unsigned int
#endif

#ifndef DATACONN_H_BBS
#define DATACONN_H_BBS
class dbconn
{
private:
	MYSQL			m_SQLConn; 
	MYSQL_RES		*m_SQLResult; //用于保存查询后的结果集
	MYSQL_ROW		m_SQLRow; 
	MYSQL_FIELD		*m_SQLField;
	int			m_fieldCount;//查询返回表的列数
	int			m_rowCount;//查询返回表的行数
	bool			m_bConnect;

	char			*m_szHost; 
	char			*m_szDB; 
	char			*m_szUser; 
	char			*m_szPass;
	uint			m_szPort;//连接端口
	
public:
	dbconn(char* szHost,char* szDB,char* szUser,char* szPass,uint szPort)//connect to mysql
	{
		this->m_szHost=szHost;
		this->m_szDB=szDB;
		this->m_szUser=szUser;
		this->m_szPass=szPass;
		this->m_szPort = szPort;
		this->m_bConnect = false;
	}

	int printArg()  //just for test
	{
		printf("*************%s\n",this->m_szHost);
		return true;
	}

public:
	bool connect();
	bool FreeResult();
	bool StoreResult();
	char** FetchRow();//获取当前行
	bool close();
	void output();
	ulong GetAffectedRows();//返回结果的行数
	ulong GetResultRows();//获得结果返回的行数
	uint GetFieldsCount();//获得返回的列数
	void DataSeek(uint offset);//将游标移动到offset行
	MYSQL_ROW_OFFSET GetCursor();//得到光标的当前位置，注意不是游标，不是行号
	ulong* GetFieldsLengthArray();//得到返回表的每一列的长度数组
	void GetFieldsName();//获得返回的所有列的名称

	MYSQL_ROW* SelectEx(const char* szSQL);
	bool Select(const char* szSQL);//根据传入的sql语句进行操作
	ulong select(const char* szSQL, vector<string> &vSql);
	bool Insert(const char* szSQL);
	bool Update(const char* szSQL);
	bool Delete(const char* szSQL);
	ulong getVisiteHostCount(char* hostName, char* startYear, char* endYear, char* url);
	ulong getRecordNum(char* szSQL);
	ulong getLimitNum(char* szSQL);
	char* getEarlyAndLateTime(int type, char* url, char* startYear, char* endYear);//0 early, 1 late
	bool ExecuteSQL(char* szSql, vector<string> & vSql);
	
	bool ExecuteSQL(char* szSql, int & affectedRows);  

public:
	~dbconn()
	{
		if (m_bConnect) {
			close();
		}
	}
};
#endif
