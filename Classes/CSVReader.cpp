#include "CSVReader.h"
#include "sqlite3.h"
#include "FunctionUtils.h"
#include "DBUtil.h"
//[1] 引入头文件 命名空间
#include "CSVParser.h"
using namespace CSVParser;

CSVReader* CSVReader::instance = nullptr;

CSVReader::CSVReader()
{
	
}

CSVReader::~CSVReader()
{

}

CSVReader* CSVReader::getInstance(){
	if (!instance){
		instance = new CSVReader();
	}

	return instance;
}

void CSVReader::destoryInstance(){
	CC_SAFE_DELETE(instance);
}

/**
 *先读取CSV中的内容 然后将字段填入sqlite3中
 *@csvFileName	csv文件名称
 *@tableName	写入到数据库的表名 
 *现在实现了表中含有all、dialog字符串的表的创建，例如all_boy、all_girl、 boy_dialog、girl_dialog的表的创建
 *每个表的字段可能不同 故创建表要进行判断
 */
void CSVReader::CsvToSqlite3(const char* csvFileName, std::string tableName){
	// 整理思路：
	// 表中的字段是从csv的第一行数据header中取的
	// 解析csv文件
	// [2]先获取csv文件的路径
	std::string path = csvFileName;
	// [3]解析csv文件
	Csv csv = Csv(csvFileName);
	// [4]获取总行数(包含列头)、总列数
	log("all row = %d", csv.getRowCount());
	log("all column = %d", csv.getColumnCount());
	// 判断是否存在表
	bool isExistTable = DBUtil::getInstance()->isExistTableByName(tableName);
	if (!isExistTable){	// 如果不存在的话，就创建一个表
		std::string::size_type idx = tableName.find("all", 0);	//查找表名含有all的表
		if (idx != std::string::npos){
			// 创建表
			std::string sql = StringUtils::format("create table %s(%s integer primary key autoincrement, %s text, %s text, %s integer, %s text, %s text, %s text)",
				tableName.c_str(), csv.getHeader().at(0).c_str(), csv.getHeader().at(1).c_str(), csv.getHeader().at(2).c_str(), csv.getHeader().at(3).c_str(), csv.getHeader().at(4).c_str(), csv.getHeader().at(5).c_str(), csv.getHeader().at(6).c_str());
			log(sql.c_str());
			DBUtil::getInstance()->createTable(sql, tableName);
		}
		idx = tableName.find("dialog", 0);						//查找表名含有dialog的表
		if (idx != std::string::npos){
			// 创建表
			std::string sql = StringUtils::format("create table %s(%s integer, %s integer, %s text, %s text, %s integer)",
				tableName.c_str(), csv.getHeader().at(0).c_str(), csv.getHeader().at(1).c_str(), csv.getHeader().at(2).c_str(), csv.getHeader().at(3).c_str(), csv.getHeader().at(4).c_str());
			DBUtil::getInstance()->createTable(sql, tableName);
		}
	}
	// [5]获取所有数据(第0行为列头字段)
	for (int i = 0; i < csv.getRowCount(); i++){
		//csv[i].size();// 每行数据有多少字段 也就是列
		for (int j = 0; j < csv[i].size(); j++){
			//log("%s", csv[i][j].c_str());
			if (i == 0){	// 列头字段 id head name age sex info fail_word 也可以用csv.getHeader()获取列头字段
				log("%s", csv[i][j].c_str());
			}
		}
		if (i == 0){
			continue;
		}
		// 插入数据 id相同时并且id为主键时不会插入数据
		std::string::size_type idx = tableName.find("all", 0);	//查找表名含有all的表
		if (idx != std::string::npos){
			// 插入数据
			std::string sql = StringUtils::format("insert into %s values(%s, '%s', '%s', %s, '%s', '%s', '%s')",
				tableName.c_str(), csv[i][0].c_str(), csv[i][1].c_str(), csv[i][2].c_str(), csv[i][3].c_str(), csv[i][4].c_str(), csv[i][5].c_str(), csv[i][6].c_str());
			DBUtil::getInstance()->insertData(sql);
		}
		idx = tableName.find("dialog", 0);						//查找表名含有dialog的表
		if (idx != std::string::npos){
			// 插入数据
			std::string sql = StringUtils::format("insert into %s values(%s, %s, '%s', '%s', %s)",
				tableName.c_str(), csv[i][0].c_str(), csv[i][1].c_str(), csv[i][2].c_str(), csv[i][3].c_str(), csv[i][4].c_str());
			DBUtil::getInstance()->insertData(sql);
		}
	}
	//[6] 也可以根据列头名称，获取数据
	//	log("%s\n", csv[2]["name"].c_str());
	//	//[7] 获取某一行数据
	//	Row row = csv[4];
	//	log("%s\n", row["age"].c_str());
}