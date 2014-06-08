// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#ifndef __XSRV_LOG_H__
#define __XSRV_LOG_H__

#pragma warning( disable : 4786 )

#include <string>

#include <pthread.h>

#define LOG_FATAL   0		/* 致命错误， 最高级别的日志 */
#define LOG_ERROR   1		/* 一般错误 */
#define LOG_WARN    2		/* 警告信息 */
#define LOG_INFO    3		/* 一般输出信息 */
#define LOG_DEBUG   4		/* 调试信息 */

class xsrv_log
{
public:
	xsrv_log(int dwLevel		= LOG_INFO,
		const char* pcLogPath	= ".",
		const char* pcBaseFileName = NULL,
		int dwMaxLogSize		= 1<<20,
		int dwMaxLogNum			= 10,
		bool bMultiFile = false);
	~xsrv_log();

	/** 打印普通日志 */
	void log (int dwLogLevel, const char* pcFileName, int dwLine, const char* fmt, ...);

	/** 打印16进制日志 */
	void hexlog(const char* file_name, const char* buf, int length);

protected:

	/** 检查是否需要切换日志文件 */
	int shiftfiles (const char* file_name);

	/** 得到某个序号的日志文件名 */
	std::string getlogfilename(const char* pcProfixName, int dwCurIndex);

	/** 得到该级别的日志文件的名字 */
	std::string getlogfilename(int dwLogLevel);

private:

	/** 日志文件的输出路径 */
	char m_szLogFilePath[256];

	/** 输出哪种级别的日志，比level级别高的都输出 */
	int m_dwMinLogLevel;

	/** 每个日志文件的最大大小 */
	int m_dwMaxLogSize;

	/** 日志文件的最大数目 */
	int m_dwMaxLogNum;

	bool m_bMultiFile;

	/** 日志文件的名字 */
	char m_szBaseFileName[32];

private:

	// 线程锁
	pthread_rwlock_t m_rwl;

private:

	// 日志锁初始化
	void _InitLock();

	// 写日志的时候的写锁
	void _Lock();

	// 写完日志的时候的解锁
	void _UnLock();

	// 对象析构的时候的把锁析构掉
	void _DestoryLock();
};

extern xsrv_log* g_xsrv_log;

#define XSRV_INIT_LOG if(!g_xsrv_log) g_xsrv_log = new xsrv_log
#define XSRV_LOG if(g_xsrv_log) g_xsrv_log->WriteLog
#define XSRV_HEXLOG if(g_xsrv_log) g_xsrv_log->WriteHexLog

#define FATAL_LEVEL   LOG_FATAL, __FILE__, __LINE__
#define ERROR_LEVEL   LOG_ERROR, __FILE__, __LINE__
#define WARN_LEVEL    LOG_WARN, __FILE__, __LINE__
#define INFO_LEVEL    LOG_INFO, __FILE__, __LINE__
#define DEBUG_LEVEL   LOG_DEBUG, __FILE__, __LINE__

#endif

/*if(g_pLog)
int main()
{
	//初始化日志系统
	INIT_LOG(LOG_INFO,"../log/",NULL,1<<20,10,false);

	//写日志
	LOG(LOG_ERROR,"%s,%d","谁是大影响",1);
}
*/

/** 本文件结束 */
