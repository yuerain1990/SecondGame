//  DBUtil.cpp
//  SecondGame
//
//  Created by yuerain on 15/05/19.
//
//
 
#include "DBUtil.h"
 
static DBUtil* s_pInstance = NULL;
 
const std::string cDBName = "data.db";
 
#pragma mark <构造 && 析构>
 
DBUtil::DBUtil():m_pDataBase(NULL) {
     
}
 
DBUtil::~DBUtil() {
     
}
 
#pragma mark <创建 && 销毁单例>
 
DBUtil* DBUtil::getInstance() {
    if (!s_pInstance) {
        s_pInstance = new DBUtil();
    }
    return s_pInstance;
}
 
void DBUtil::destoryInstance() {
    delete s_pInstance;
    s_pInstance = NULL;
}
 
#pragma mark <数据库操作>
 
/**
 *  打开数据库(创建)
 *
 *  @param aDataBaseName 数据库名
 */
void DBUtil::openDBWithName(std::string aDataBaseName) {
    std::string writeablePath = FileUtils::getInstance()->getWritablePath();
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(aDataBaseName);
    CCLOG("path=%s", writeablePath.c_str());
    std::string dbPath = writeablePath + aDataBaseName;
     
    int result = sqlite3_open(aDataBaseName.c_str(), &m_pDataBase);
    char* errMsg = NULL;
    if (result != SQLITE_OK) {
        log("打开数据库失败，错误码:%d，错误原因:%s\n", result, errMsg);
    }
     
    if (errMsg) {
        sqlite3_free(errMsg);
        errMsg = NULL;
    }
}
 
/**
 *  关闭数据库
 */
void DBUtil::closeDB() {
    if (m_pDataBase) {
        sqlite3_close(m_pDataBase);
        m_pDataBase = NULL;
    }
}
 
/**
 *  创建数据表
 *
 *  @param aSql       建表sql语句
 *  @param aTableName 表名
 *
 *  @usage string sql = "create table user(id integer, username text, password text)";
 */
void DBUtil::createTable(std::string aSql, std::string aTableName) {
     
    //openDBWithName(cDBName);
     
    if (!isExistTableByName(aTableName)) {
        char* errMsg = NULL;
        int result = sqlite3_exec(m_pDataBase, aSql.c_str(), NULL, NULL, &errMsg);
        if (result != SQLITE_OK) {
            log("创建表失败，错误码:%d，错误原因:%s\n", result, errMsg);
        }
         
        if (errMsg) {
            sqlite3_free(errMsg);
            errMsg = NULL;
        }
    }
     
    //closeDB();
}
 
/**
 *  是否存在某张数据表的查询回调
 *
 *  @return 0
 */
int isExistTableCallback(void* para, int n_column, char ** column_value, char ** column_name) {
    bool *isExisted_= (bool*)para;
    *isExisted_= (**column_value) != '0';
    return 0;
}
 
/**
 *  通过表名查询该表是否存在
 *
 *  @param aTabelName 表秒
 *
 *  @return true: 存在 false: 不存在
 */
bool DBUtil::isExistTableByName(std::string aTabelName) {
     
    if (m_pDataBase) {
        //判断表是否存在
        bool isExist;
        char* errMsg = NULL;
        std::string sql = "select count(type) from sqlite_master where type = 'table' and name = '" + aTabelName + "'";
        int result = sqlite3_exec(m_pDataBase, sql.c_str(), isExistTableCallback, &isExist, &errMsg);
         
        if (result != SQLITE_OK) {
            log("查询表是否存在失败，错误码:%d，错误原因:%s\n", result, errMsg);
        }
         
        if (errMsg) {
            sqlite3_free(errMsg);
            errMsg = NULL;
        }
         
        return isExist;
    }
     
    return false;
}
 
/**
 *  删除数据表
 *
 *  @param aSql       删表sql语句
 *  @param aTableName 表名
 *
 *  @usage string sql = "drop table name";
 */
void DBUtil::deleteTable(std::string aSql, std::string aTableName) {
     
    //openDBWithName(cDBName);
     
    beginTransaction();
     
    int result = 0;
    if (isExistTableByName(aTableName)) {
        char* errMsg = NULL;
        result = sqlite3_exec(m_pDataBase, aSql.c_str(), NULL, NULL, &errMsg);
        if (result != SQLITE_OK) {
            log("创建表失败，错误码:%d，错误原因:%s\n", result, errMsg);
        }
         
        if (errMsg) {
            sqlite3_free(errMsg);
            errMsg = NULL;
        }
    }
     
    commitTransaction(result);
     
    //closeDB();
     
}
 
/**
 *  插入记录
 *
 *  @param aSql 插入数据sql语句
 *
 *  @usage string sql = "insert into User(name) values ('cc') ";
 */
void DBUtil::insertData(std::string aSql) {
     
    //openDBWithName(cDBName);
     
    beginTransaction();
     
    char* errMsg = NULL;
    int result = sqlite3_exec(m_pDataBase, aSql.c_str(), NULL, NULL, &errMsg);
    if (result != SQLITE_OK) {
        log("插入记录失败，错误码:%d，错误原因:%s\n", result, errMsg );
    }
     
    if (errMsg) {
        sqlite3_free(errMsg);
        errMsg = NULL;
    }
     
    commitTransaction(result);
     
    //closeDB();
}
 
/**
 *  删除记录
 *
 *  @param aSql 插入数据sql语句
 *
 *  @usage string sql = "delete from User where name = 'cc'";
 */
void DBUtil::deleteData(std::string aSql) {
     
    //openDBWithName(cDBName);
     
    beginTransaction();
     
    char* errMsg = NULL;
    int result = sqlite3_exec(m_pDataBase, aSql.c_str(), NULL, NULL, &errMsg);
    if (result != SQLITE_OK ) {
        log("删除记录失败，错误码:%d，错误原因:%s\n" , result, errMsg);
    }
     
    if (errMsg) {
        sqlite3_free(errMsg);
        errMsg = NULL;
    }
     
    commitTransaction(result);
     
    //closeDB();
     
}
 
/**
 *  修改记录
 *
 *  @param aSql 修改数据sql语句
 */
void DBUtil::updateData(std::string aSql) {
     
    //openDBWithName(cDBName);
     
    beginTransaction();
     
    char* errMsg = NULL;
    int result = sqlite3_exec(m_pDataBase, aSql.c_str(), NULL, NULL, &errMsg );
    if (result != SQLITE_OK) {
        log( "修改记录失败，错误码:%d，错误原因:%s\n", result, errMsg );
    }
     
    if (errMsg) {
        sqlite3_free(errMsg);
        errMsg = NULL;
    }
     
    commitTransaction(result);
     
    //closeDB();
}
 
/**
 *  查询回调
 *
 *  @return 0
 */
int searchDataCallback(void* para, int n_column, char** column_value, char** column_name ) {
    std::map<std::string, std::string> mapResults ;
    for (int i = 0; i < n_column; i++) {
		if (!column_value[i])
			column_value[i] = "";
        mapResults.insert(std::map<std::string, std::string>::value_type((std::string)column_name[i], (std::string)column_value[i]));
    }
    std::vector<std::map<std::string, std::string>>* vect = (std::vector<std::map<std::string, std::string>>*)para;
    vect->push_back(mapResults);
    return 0;
}

/**
*  查询回调
*
*  @return 0
*/
int searchDataCallback2(void* para, int n_column, char** column_value, char** column_name) {
	std::map<std::string, std::string> mapResults;
	for (int i = 0; i < n_column; i++) {
		if (!column_value[i])
			column_value[i] = "";
		mapResults.insert(std::map<std::string, std::string>::value_type((std::string)column_name[i], (std::string)column_value[i]));
	}
	std::vector<std::map<std::string, std::string>>* vect = (std::vector<std::map<std::string, std::string>>*)para;
	vect->push_back(mapResults);
	return 0;
}
 
/**
 *  查询记录
 *
 *  @param aSql     查询数据sql语句
 */
std::vector<std::map<std::string, std::string>> DBUtil::searchData(std::string aSql) {
    //openDBWithName(cDBName);
     
    //vector是查询的结果集，每一个结果都存在map中
    //map的第一string是key(字段名)，第二个string是value(查询出的对应数据)
    std::vector<std::map<std::string, std::string> > vec;
     
    char* errMsg = NULL;
    int result = sqlite3_exec(m_pDataBase, aSql.c_str(), searchDataCallback, &vec, &errMsg);
    if (result != SQLITE_OK) {
        log("查询失败1，错误码:%d，错误原因:%s\n", result, errMsg);
    }
     
    if (errMsg) {
        sqlite3_free(errMsg);
        errMsg = NULL;
    }
     
    //closeDB();
     
    return vec;
}

/**
*  查询记录
*
*  @param aSql1查询记录条数 aSql2查询一条数据 aSql3插入一条数据
*/
std::vector<std::map<std::string, std::string>> DBUtil::searchData(std::string aSql1, std::string aSql2, std::string aSql3){
	//openDBWithName(cDBName);

	int count = searchDataCount(aSql1);
	if (count == 0){
		insertData(aSql3);
		searchData(aSql1, aSql2, aSql3);
	}
	//vector是查询的结果集，每一个结果都存在map中
	//map的第一string是key(字段名)，第二个string是value(查询出的对应数据)
	std::vector<std::map<std::string, std::string> > vec;

	char* errMsg = NULL;
	int result = sqlite3_exec(m_pDataBase, aSql2.c_str(), searchDataCallback2, &vec, &errMsg);
	if (result != SQLITE_OK) {
		log("查询失败2，错误码:%d，错误原因:%s\n", result, errMsg);
	}

	if (errMsg) {
		sqlite3_free(errMsg);
		errMsg = NULL;
	}

	//closeDB();

	return vec;
}
 
/**
 *  查询数据条数回调
 *
 *  @return 0
 */
int searchDataCountCallback(void* para, int n_column, char** column_value, char** column_name) {
    int* count = (int*)para;
    *count = (int)atof(column_value[0]);
    return 0;
}
 
/**
 *  查询记录的条数
 *
 *  @param sql 查询记录sql语句
 *
 *  @return 记录条数
 */
int DBUtil::searchDataCount(std::string aSql) {
     
    //openDBWithName(cDBName);
     
    int count = 0;
    char* errMsg = NULL;
    int result = sqlite3_exec(m_pDataBase, aSql.c_str(), searchDataCountCallback, &count, &errMsg);
    if (result != SQLITE_OK) {
        log( "查询记录条数失败，错误码:%d，错误原因:%s\n", result, errMsg);
    }
     
    if (errMsg) {
        sqlite3_free(errMsg);
        errMsg = NULL;
    }
     
    //closeDB();
     
    return count;
}
 
/**
 *  开始事务
 *
 *  @return 操作结果(sqlite3提供的宏)
 */
int DBUtil::beginTransaction() {
    char* errMsg = NULL;
    int result = sqlite3_exec(m_pDataBase, "begin transaction", 0, 0, &errMsg);
    if (result != SQLITE_OK ){
        log("开始事务记录失败，错误码:%d，错误原因:%s\n", result, errMsg);
    }
    return result;
}
 
/**
 *  提交事务(失败回滚)
 *
 *  @param aResult       操作结果
 *
 *  @return 操作结果(sqlite3提供的宏)
 */
int DBUtil::commitTransaction(int aResult) {
    if (aResult == SQLITE_OK) {
        char* errMsg = NULL;
        int result = sqlite3_exec(m_pDataBase, "commit transaction", 0, 0, &errMsg);
        if (result != SQLITE_OK) {
            log("提交事务记录失败，错误码:%d，错误原因:%s\n" , result, errMsg);
        }
        return result;
    } else {
        char* errMsg = NULL;
        int result = sqlite3_exec(m_pDataBase, "rollback transaction", 0, 0, &errMsg);
        if (result != SQLITE_OK ) {
            log("回滚事务记录失败，错误码:%d，错误原因:%s\n", result, errMsg);
        }
        return result;
    }
}