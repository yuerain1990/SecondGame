#include "CSVReader.h"
#include "sqlite3.h"
#include "FunctionUtils.h"
#include "DBUtil.h"
//[1] ����ͷ�ļ� �����ռ�
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
 *�ȶ�ȡCSV�е����� Ȼ���ֶ�����sqlite3��
 *@csvFileName	csv�ļ�����
 *@tableName	д�뵽���ݿ�ı��� 
 *����ʵ���˱��к���all��dialog�ַ����ı�Ĵ���������all_boy��all_girl�� boy_dialog��girl_dialog�ı�Ĵ���
 *ÿ������ֶο��ܲ�ͬ �ʴ�����Ҫ�����ж�
 */
void CSVReader::CsvToSqlite3(const char* csvFileName, std::string tableName){
	// ����˼·��
	// ���е��ֶ��Ǵ�csv�ĵ�һ������header��ȡ��
	// ����csv�ļ�
	// [2]�Ȼ�ȡcsv�ļ���·��
	std::string path = csvFileName;
	// [3]����csv�ļ�
	Csv csv = Csv(csvFileName);
	// [4]��ȡ������(������ͷ)��������
	log("all row = %d", csv.getRowCount());
	log("all column = %d", csv.getColumnCount());
	// �ж��Ƿ���ڱ�
	bool isExistTable = DBUtil::getInstance()->isExistTableByName(tableName);
	if (!isExistTable){	// ��������ڵĻ����ʹ���һ����
		std::string::size_type idx = tableName.find("all", 0);	//���ұ�������all�ı�
		if (idx != std::string::npos){
			// ������
			std::string sql = StringUtils::format("create table %s(%s integer primary key autoincrement, %s text, %s text, %s integer, %s text, %s text, %s text)",
				tableName.c_str(), csv.getHeader().at(0).c_str(), csv.getHeader().at(1).c_str(), csv.getHeader().at(2).c_str(), csv.getHeader().at(3).c_str(), csv.getHeader().at(4).c_str(), csv.getHeader().at(5).c_str(), csv.getHeader().at(6).c_str());
			log(sql.c_str());
			DBUtil::getInstance()->createTable(sql, tableName);
		}
		idx = tableName.find("dialog", 0);						//���ұ�������dialog�ı�
		if (idx != std::string::npos){
			// ������
			std::string sql = StringUtils::format("create table %s(%s integer, %s integer, %s text, %s text, %s integer)",
				tableName.c_str(), csv.getHeader().at(0).c_str(), csv.getHeader().at(1).c_str(), csv.getHeader().at(2).c_str(), csv.getHeader().at(3).c_str(), csv.getHeader().at(4).c_str());
			DBUtil::getInstance()->createTable(sql, tableName);
		}
	}
	// [5]��ȡ��������(��0��Ϊ��ͷ�ֶ�)
	for (int i = 0; i < csv.getRowCount(); i++){
		//csv[i].size();// ÿ�������ж����ֶ� Ҳ������
		for (int j = 0; j < csv[i].size(); j++){
			//log("%s", csv[i][j].c_str());
			if (i == 0){	// ��ͷ�ֶ� id head name age sex info fail_word Ҳ������csv.getHeader()��ȡ��ͷ�ֶ�
				log("%s", csv[i][j].c_str());
			}
		}
		if (i == 0){
			continue;
		}
		// �������� id��ͬʱ����idΪ����ʱ�����������
		std::string::size_type idx = tableName.find("all", 0);	//���ұ�������all�ı�
		if (idx != std::string::npos){
			// ��������
			std::string sql = StringUtils::format("insert into %s values(%s, '%s', '%s', %s, '%s', '%s', '%s')",
				tableName.c_str(), csv[i][0].c_str(), csv[i][1].c_str(), csv[i][2].c_str(), csv[i][3].c_str(), csv[i][4].c_str(), csv[i][5].c_str(), csv[i][6].c_str());
			DBUtil::getInstance()->insertData(sql);
		}
		idx = tableName.find("dialog", 0);						//���ұ�������dialog�ı�
		if (idx != std::string::npos){
			// ��������
			std::string sql = StringUtils::format("insert into %s values(%s, %s, '%s', '%s', %s)",
				tableName.c_str(), csv[i][0].c_str(), csv[i][1].c_str(), csv[i][2].c_str(), csv[i][3].c_str(), csv[i][4].c_str());
			DBUtil::getInstance()->insertData(sql);
		}
	}
	//[6] Ҳ���Ը�����ͷ���ƣ���ȡ����
	//	log("%s\n", csv[2]["name"].c_str());
	//	//[7] ��ȡĳһ������
	//	Row row = csv[4];
	//	log("%s\n", row["age"].c_str());
}