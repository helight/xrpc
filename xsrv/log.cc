#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "log.h"

const char g_szLogLevel[5][6] = {"FATAL", "ERROR","WARN", "INFO", "DEBUG"};
xsrv_log* g_xsrv_log = NULL;

using namespace std;

xsrv_log::xsrv_log (int dwLevel,
		const char* pcLogPath,
		const char* pcBaseFileName,
		int dwMaxLogSize,
		int dwMaxLogNum,
		bool bMultiFile
		)
{
	if(dwLevel < 0 || dwLevel > LOG_DEBUG)
		dwLevel = LOG_INFO;
	m_dwMaxLogSize  = dwMaxLogSize;
	m_dwMaxLogNum   = dwMaxLogNum;
	m_dwMinLogLevel = dwLevel;
	m_bMultiFile = bMultiFile;
	strncpy (m_szLogFilePath, pcLogPath, 255);
	if (pcBaseFileName == NULL)
		strncpy(m_szBaseFileName,"tty",sizeof(m_szBaseFileName));
	else
		strncpy (m_szBaseFileName, pcBaseFileName, sizeof(m_szBaseFileName));

	// 线程锁初始化
	_InitLock();
}

xsrv_log::~xsrv_log() 
{
	// 对象析构的时候的把锁析构掉
	_DestoryLock();
};

/** 得到某个序号的日志文件名 */
string xsrv_log::getlogfilename(const char* pcProfixName,int dwCurIndex)
{
	char szIndex[10];
	string 	strFileName = pcProfixName;

	if (dwCurIndex == 0)
		strFileName += ".log";
	else { 
		sprintf(szIndex, "_%d", dwCurIndex);		
		strFileName += string (szIndex) + ".log";		
	}

	return strFileName;
}

/** 得到该级别的日志文件的名字 */
string xsrv_log::getlogfilename(int dwLogLevel)
{
	string 	strFileName= m_szLogFilePath + string("/") +string(m_szBaseFileName);

	// 是否使用多个日志文件
	if (m_bMultiFile)
		strFileName += string("_")+string(g_szLogLevel[dwLogLevel]) ;

	char	szDate[10];
	time_t now = time (NULL);
	struct tm* tm;
	tm = localtime(&now);
	snprintf(szDate, sizeof(szDate), "_%04d%02d%02d",tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

	strFileName += string(szDate);

	return strFileName;
}

/** 打印16进制日志 */
void xsrv_log::hexlog (const char* pcFileName, const char* pcBuf, int dwLength)
{
	FILE* fp = NULL;

	_Lock();
	if ((fp = fopen(pcFileName, "w+b"))) {
		fwrite(pcBuf, dwLength, 1, fp);
		fclose(fp); 
	}
	_UnLock();
}

/** 打印普通日志 */
void xsrv_log::log (int dwLogLevel, const char* pcFileName, int dwLine,const char* fmt, ...)
{
	if (dwLogLevel > m_dwMinLogLevel)
		return ;

	string strFileName = getlogfilename (dwLogLevel);
	if (strFileName.empty ())
		return ;

	shiftfiles (strFileName.c_str ());
	strFileName += ".log";

	char szLogBuf[1024];
	long dwPos =0;


	long now =time(NULL);
	struct tm* tm;
	tm = localtime(&now);

	// 打印日志时间、进程号、文件、行号、级别
	dwPos += snprintf(szLogBuf+dwPos, sizeof(szLogBuf)-dwPos, 
			"[%04d-%02d-%02d %02d:%02d:%02d] (%d) [%s: %d] %s : ",
			tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
			getpid(),pcFileName,dwLine,g_szLogLevel[dwLogLevel]);

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(szLogBuf + dwPos, sizeof(szLogBuf) - dwPos, fmt, ap);
	va_end(ap);


	_Lock();	
	// 打印到屏幕
#ifdef _BOTHOUT
	printf("%s\n", szLogBuf);
#endif
	FILE *fp = fopen(strFileName.c_str(), "at");
	if (fp) {
		fprintf (fp, "%s\n", szLogBuf);
		fclose (fp);
	}
	_UnLock();
}

/** 检查是否需要切换日志文件 */
int xsrv_log::shiftfiles (const char* pcFileName)
{
	string strTmp,strNewFile;
	struct stat oFileStat;

	// 检查是否需要切换日志文件
	strTmp = getlogfilename (pcFileName, 0);			
	if (stat (strTmp.c_str(), &oFileStat) < 0 || oFileStat.st_size < m_dwMaxLogSize)
		return 0;

	// 删除最旧的日志文件
	strTmp = getlogfilename (pcFileName, m_dwMaxLogNum - 1);
	if (access (strTmp.c_str(), F_OK) == 0 && remove (strTmp.c_str ()) < 0)
		return -1;

	// 文件重新命名
	for (int i = m_dwMaxLogNum - 2; i >= 0; i--) {
		strTmp = getlogfilename(pcFileName, i);			
		if (access(strTmp.c_str(), F_OK) == 0) {
			strNewFile = getlogfilename(pcFileName, i + 1);	
			if (rename(strTmp.c_str(),strNewFile.c_str()) < 0 )
				return -1;
		}
	}

	return 0;	
}

// 日志锁初始化
void xsrv_log::_InitLock()
{
	pthread_rwlock_init(&m_rwl, NULL);
}

// 写日志的时候的写锁
void xsrv_log::_Lock()
{
	pthread_rwlock_wrlock(&m_rwl);
}

// 写完日志的时候的解锁
void xsrv_log::_UnLock()
{
	pthread_rwlock_unlock(&m_rwl);
}

// 对象析构的时候的把锁析构掉
void xsrv_log::_DestoryLock()
{
	// 线程锁初始化
	pthread_rwlock_destroy(&m_rwl);
}

/** 本文件结束 */
