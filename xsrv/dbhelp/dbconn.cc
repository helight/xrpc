// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-06-08
// Description:
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dbconn.h"

bool dbconn::connect()
{
	mysql_init(&this->m_SQLConn);

	if (mysql_real_connect(&this->m_SQLConn, this->m_szHost, this->m_szUser,
				this->m_szPass, this->m_szDB, this->m_szPort, NULL, 0) == NULL) {
		printf("mysql_real_connect fail\n");
		return false;
	}
	mysql_query(&m_SQLConn, "SET NAMES utf8");

	this->m_bConnect = true;
	return true;
}


bool dbconn::Select(const char* szSQL)
{
	bool success = false;
	if (szSQL == NULL) return false;
	if (connect()) {
		int Ret = mysql_query(&this->m_SQLConn, szSQL);
		if (Ret != 0) {//失败
			//printf("Content-type:text/html;charset=utf-8 \n\n");

			printf("<MySQL-WARNING> DB real query fail:%s</MySQL-WARNING>\n", mysql_error(&this->m_SQLConn));
			success = false;
		} else {
			success = true;
		}
	}
	return success;
}

ulong dbconn::select(const char* szSQL, vector<string> &vSql)
{
	ulong count = 0;
	if (szSQL == NULL) return count;
	char* p = strstr((char *)szSQL, "from");
	if ( p == NULL ) return count;
	//找有多少个字段
	int dwRow=1;
	char *q = (char *)szSQL;
	while (q != p) {
		if (*q == ',') {
			++dwRow;
		}
		++q;
	}
	if (connect()){
		int Ret = mysql_query(&this->m_SQLConn, szSQL);
		if (Ret != 0) {//失败
			count = 0;
		} else {
			StoreResult();
			count =  GetResultRows();
			for (int i = 0; i != count; i++) {
				this->m_SQLRow = mysql_fetch_row(this->m_SQLResult);
				for (int j = 0; j < dwRow; j++) {
					if (this->m_SQLRow[j] == NULL)
						vSql.push_back("");
					else
						vSql.push_back(string(this->m_SQLRow[j]));
				}
			}
			FreeResult();
			close();
		}
	}
	return count;
}

ulong dbconn::getRecordNum(char* szSQL)
{
	ulong count = 0;
	if (Select(szSQL)) {
		StoreResult();
		if (FetchRow()) {
			count = strtoul(m_SQLRow[0], NULL, 10);
		}
		FreeResult();
		close();
	}
	return count;
}

void dbconn::output()  /*just print the result*/
{
	if (this->m_SQLRow != NULL) {
		while (FetchRow()) {
			for (int j = 0; j<m_fieldCount; j++) {
				printf("%s",m_SQLRow[j]);
			}
			printf("\n");
		}
	}
}

bool dbconn::FreeResult()
{
	if (this->m_SQLResult != NULL) {
		mysql_free_result(this->m_SQLResult);
		this->m_SQLResult = NULL ;
		this->m_fieldCount = 0;
	}
	return true;
}

bool dbconn::StoreResult()
{
	bool b = false;
	this->m_SQLResult = mysql_store_result(&this->m_SQLConn);
	if (this->m_SQLResult == NULL) {
		//printf ("<***MySQL ERROR***> DBSaveResult ERR:%s!\r", mysql_error(&this->m_SQLConn));
		m_fieldCount=0;
	} else {
		m_fieldCount=mysql_num_fields(this->m_SQLResult);
		b=true;
	}
	return b;
}

char** dbconn::FetchRow()
{
	this->m_SQLRow = 0;
	if(this->m_SQLResult != NULL)
		this->m_SQLRow = mysql_fetch_row(this->m_SQLResult);
	return this->m_SQLRow;
}

bool dbconn::Insert(const char* szSQL)
{
	bool sucess = false;
	if (szSQL == NULL) return false;
	if (connect()) {
		int Ret = mysql_query(&this->m_SQLConn, szSQL);
		if (Ret != 0) {//失败
			printf("<MySQL-WARNING> insert into db fail:%s</MySQL-WARNING>\n", mysql_error(&this->m_SQLConn));
			sucess = false;
		} else
			sucess = true;
		close();
	}
	return sucess;
}

bool dbconn::Update(const char* szSQL)
{
	bool sucess = false;
	if (szSQL == NULL) return false;
	if (connect()) {
		int Ret = mysql_query(&this->m_SQLConn, szSQL);
		if (Ret != 0) {//失败
			printf("<MySQL-WARNING> update db fail:%s</MySQL-WARNING>\n", mysql_error(&this->m_SQLConn));
		} else
			sucess = true;
		close();
	}
	return sucess;
}

bool dbconn::Delete(const char* szSQL)
{
	bool sucess = false;
	if (szSQL == NULL) return false;
	if (connect()) {
		int Ret = mysql_query(&this->m_SQLConn,szSQL);
		if (Ret != 0){//失败
			printf("<MySQL-WARNING> delete from db fail:%s</MySQL-WARNING>\n", mysql_error(&this->m_SQLConn));
		}else
			sucess = true;
		close();
	}
	return sucess;
}

ulong* dbconn::GetFieldsLengthArray()
{
	if (m_SQLResult != NULL) {
		return mysql_fetch_lengths(m_SQLResult);
	}
	return NULL;
}

void dbconn::DataSeek(uint offset)
{
	if (m_SQLResult != NULL && offset >= 0)	{
		mysql_data_seek(m_SQLResult,offset);
	}
}

MYSQL_ROW_OFFSET dbconn::GetCursor()
{
	if (m_SQLResult != NULL) {
		return mysql_row_tell(m_SQLResult);
	}
	return NULL;
}

uint dbconn::GetFieldsCount()
{
	if (m_SQLResult != NULL) {
		return mysql_num_fields(m_SQLResult);
	}
	return 0;
}

void dbconn::GetFieldsName()
{
	if (m_SQLResult != NULL) {
		m_SQLField = mysql_fetch_fields(m_SQLResult);
		for (uint i = 0;i < GetFieldsCount(); i++) {
			printf("Field %u is %s\n", i, m_SQLField[i].name);
		}
	}
}

ulong dbconn::GetAffectedRows()
{
	if (&m_SQLConn != NULL)	{
		return (ulong)mysql_affected_rows(&m_SQLConn);
	}
	return 0;
}
ulong dbconn::getLimitNum(char* szSQL)
{
	ulong count = 0;
	if (szSQL == NULL) {
		return count;
	}

	if (Select(szSQL)) {
		StoreResult();
		count = GetResultRows();
		FreeResult();
		close();
	}
	return count;
}

ulong dbconn::GetResultRows()
{
	if (m_SQLResult != NULL) {
		return (ulong)mysql_num_rows(m_SQLResult);
	}
	return 0;
}

bool dbconn::close()
{
	if (&this->m_SQLConn == NULL) return false;
	mysql_close(&this->m_SQLConn);
	this->m_bConnect = false;

	return true;
}

bool dbconn::ExecuteSQL(char* szSQL, vector<string> & vSql)
{
	if (szSQL == NULL)
		return false;
	char* p = strstr(szSQL, "from");
	if ( p == NULL ) {
		printf( "can not find from");
		return false;
	}

	//找有多少个字段
	int dwRow = 1;
	char *q = szSQL;
	while (q != p) {
		if (*q == ',') {
			++dwRow;
		}
		++q;
	}
	//printf(" select colNum=%d\n",dwRow);

	// 执行相应的SQL语句
	int ret = mysql_query(&this->m_SQLConn, szSQL);
	if (ret != 0) {
		printf("Fail To Execute SQL: %s", mysql_error(&this->m_SQLConn));
		return false;
	}

	this->m_SQLResult = mysql_store_result(&this->m_SQLConn);
	if (this->m_SQLResult == NULL) {
		m_rowCount = 0;
	} else {
		m_rowCount=mysql_num_rows(this->m_SQLResult);
	}

	for (int i = 0; i != m_rowCount; i++) {
		this->m_SQLRow = mysql_fetch_row(this->m_SQLResult);
		for (int j = 0; j < dwRow; j++) {
			if ( this->m_SQLRow[j] == NULL )
				vSql.push_back("");
			else
				vSql.push_back(string(this->m_SQLRow[j]));
		}
	}

	mysql_free_result(this->m_SQLResult);
	return true;
}


bool dbconn::ExecuteSQL(char* szSQL, int & affectedRows )
{
	if (szSQL == NULL)
		return false;

	int ret = mysql_query(&this->m_SQLConn,szSQL);
	if (ret != 0) {
		printf( "Fail To Execute SQL: %s", mysql_error(&this->m_SQLConn)  );
		return false;
	}
	affectedRows = mysql_affected_rows(&this->m_SQLConn);

	this->m_SQLResult = mysql_store_result(&this->m_SQLConn);
	if (this->m_SQLResult == NULL) {
		//printf("m_SQLResult == NULL\n");
		m_rowCount = 0;
		//affectedRows=0;
	} else {
		//printf("m_SQLResult ==Not NULL\n");
		m_rowCount = mysql_num_rows(this->m_SQLResult);
		//affectedRows=m_rowCount;
	}

	mysql_free_result(this->m_SQLResult);
	return true;
}
