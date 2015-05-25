#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CSVReader
{
public:
	CSVReader();
	~CSVReader();

	static CSVReader* getInstance();
	static void destoryInstance();

	void CsvToSqlite3(const char* csvFileName, std::string tableName);
private:
	static CSVReader* instance;
};
