#include "parse_conf.h"

CReadIni::CReadIni()
{
}

CReadIni::~CReadIni()
{
}

int CReadIni::LoadConfigFile(const char* pFileName)
{
	if (pFileName ==NULL || pFileName[0] ==0)
		return PARAM_ERROR;

	FILE* fp = fopen(pFileName,"r");
	if (fp ==NULL)
		return FILE_OPEN_ERROR;
	char* pBuf = new char[MAX_LENGTH+2];
	if (pBuf ==NULL)
		return MEM_NOT_ENOUGH;
	memset(pBuf,0,MAX_LENGTH+2);


	string section;
	string key;
	string value;

	while (fgets(pBuf,MAX_LENGTH,fp)) {
		if (*pBuf == '#')
			continue;
		char* p = pBuf;
		while (*p && ( *p ==' ' || *p =='\t'  || *p == '\r' ||  *p == '\n') )
			p++;
		if (*p =='\0')
			continue;
		if (*p == '[') { //section
			p++;
			GetSection(p,section);
		} else { 
			GetKey(p,key,value);
			if(section.length() >0 && key.length() >0){
				AddToMap(section,key,value);
			}
		}
	}

	fclose(fp);
	delete [] pBuf;
	return SUCCEEDED;
}
int CReadIni::AddToMap(string section,string key,string value)
{
	map< string ,map <string ,string > >::iterator it = m_mapConfig.find(section);
	if (it != m_mapConfig.end()) {
		it->second.insert(make_pair(key,value));
	} else {
		map <string ,string >  mapKey;
		mapKey.insert(make_pair(key,value));
		m_mapConfig.insert(make_pair(section,mapKey));
	}
	return SUCCEEDED;
}

int CReadIni::GetSection(char* pBuf, string & strSection)
{
	return GetStr(pBuf,strSection,']');
}

int CReadIni::GetKey(char* pBuf, string & strKey, string & strValue)
{
	GetStr(pBuf,strKey,'=');
	if (*pBuf)
		pBuf++;
	GetValue(pBuf,strValue);
	return SUCCEEDED;
}

int CReadIni::GetValue(char* pBuf, string & strValue)
{
	while (*pBuf && (*pBuf ==' ' || *pBuf =='\t'  || *pBuf == '\r' ||  *pBuf == '\n'))
		pBuf++;
	long l = strlen(pBuf);
	if (l == 0)
		return FAILED;
	while (l > 0 && (*(pBuf+l-1) == ' ' || *(pBuf+l-1) == '\t' || *(pBuf+l-1) == '\r' || *(pBuf+l-1) == '\n' ))
		l--;
	*(pBuf+l) =0;
	strValue = string(pBuf);
	return SUCCEEDED;
}

int CReadIni::GetStr(char* &pBuf, string & str,char chEnd)
{
	int i=0;
	char* pTmp = new char[MAX_LENGTH+2];
	memset(pTmp,0,MAX_LENGTH+2);
	while (pBuf && *pBuf && *pBuf != chEnd && i <MAX_LENGTH) {
		pTmp[i++] = CharToLower(*pBuf++);
	}

	//去掉str末尾的空格 tab 换行等符号
	if (i ==0) {
		delete [] pTmp;
		return FAILED;
	}
	while (i>0 && ( pTmp[i-1] ==' ' || pTmp[i-1] =='\r' || pTmp[i-1] =='\t' || pTmp[i-1] =='\n')) {
		i--;
	}
	pTmp[i]=0;
	pTmp[i+1]=0;
	str  =string(pTmp);

	delete [] pTmp;

	return SUCCEEDED;
}

int CReadIni::GetConfigStr(const char* pSection,const char* pKey,char* pOutBuf,unsigned long dwBufLen)
{
	if (pOutBuf ==NULL)
		return FAILED;
	pOutBuf[0] =0;

	map< string ,map <string ,string > >::iterator it = m_mapConfig.find(ToLower(pSection));
	if (it ==m_mapConfig.end())
		return SEC_NOT_FOUND;

	map<string ,string >::iterator itKey = it->second.find(ToLower(pKey));
	if (itKey == it->second.end())
		return KEY_NOT_FOUND;

	if (dwBufLen < itKey->second.length() +1)
		return BUF_NOT_ENOUTH;
	strcpy(pOutBuf,itKey->second.c_str());

	//	printf("[%s] %s = %s \n",pSection,pKey,pOutBuf);

	return SUCCEEDED;
}

int CReadIni::GetConfigInt(const char* pSection,const char* pKey)
{ 
	char buf[20];
	int ret = GetConfigStr(pSection,pKey,buf,20);
	if (ret !=0)
		return ret;
	//	printf("[%s] %s = %d \n",pSection,pKey,atoi(buf));
	return atoi(buf);
}

char CReadIni::CharToLower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;
	else
		return c;
}

/** 把字符串转为小写 */
std::string CReadIni::ToLower(const char *pszValue)
{
	std::string  strRet;
	size_t dwLen = strlen(pszValue);
	for (unsigned long i = 0 ; i < dwLen; i ++) {
		strRet += CharToLower(pszValue[i]);
	}

	return strRet;
}

