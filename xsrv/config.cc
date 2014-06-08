#include "config.h"

XAppConfig * XAppConfig::m_pAppConf = NULL;
bool XAppConfig::LoadConf(const char* pConf)
{
	int iRet = ini.LoadConfigFile(pConf);
	if (SUCCEEDED!= iRet) {
		return false;
	}

	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","localIp",myIdentification.localIp,20)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","myRole",myIdentification.myRole,20)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_ip",myIdentification.master_db_ip,20)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_user",myIdentification.master_db_user,30)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_pass",myIdentification.master_db_pass,30)) {
		return false;
	}
	if (SUCCEEDED!= ini.GetConfigStr("myIdentification","master_db_db",myIdentification.master_db_db,30)) {
		return false;
	}

	myIdentification.master_db_port = ini.GetConfigInt("myIdentification","master_db_port");
	if (myIdentification.master_db_port <= 0) {
		return false;
	}

	return true;
}

