#include "FunctionUtils.h"

/**
*��Բ�Ǿ��� ���ڵװ�Ļ����ϻ� �ʿ�ʼ�� �� �������ǵװ�����½Ǻ����Ͻ� �װ��ê�������������ĵ�����Ϊ(0, 0)
*bgSprite�װ� origin���ο�ʼ�� destination���ν����� radius�뾶 segmentsԲ�ǵȷ������ȷ�Խ�࣬Բ��Խƽ��
*/
ClippingNode* ccDrawRoundRect(Sprite *bgSprite, Vec2 origin, Vec2 destination, float radius, unsigned int segments)
{
	Sprite *thisbgSprite = bgSprite;
	ClippingNode* pClip = ClippingNode::create();

	pClip->setInverted(false);//�����Ƿ��򣬽�������������Բ��͸���Ļ��Ǻ�ɫ��
	//    this->addChild(pClip);
	pClip->setAnchorPoint(Point(0, 0));
	pClip->setPosition(-7, -7);

	//���1/4Բ
	const float coef = 0.5f * (float)M_PI / segments;
	Point * vertices = new Point[segments + 1];
	Point * thisVertices = vertices;
	for (unsigned int i = 0; i <= segments; ++i, ++thisVertices)
	{
		float rads = (segments - i)*coef;
		thisVertices->x = (int)(radius * sinf(rads));
		thisVertices->y = (int)(radius * cosf(rads));
	}
	//
	Point tagCenter;
	float minX = MIN(origin.x, destination.x);
	float maxX = MAX(origin.x, destination.x);
	float minY = MIN(origin.y, destination.y);
	float maxY = MAX(origin.y, destination.y);

	unsigned int dwPolygonPtMax = (segments + 1) * 4;
	Point * pPolygonPtArr = new Point[dwPolygonPtMax];
	Point * thisPolygonPt = pPolygonPtArr;
	int aa = 0;
	//���Ͻ�
	tagCenter.x = minX + radius;
	tagCenter.y = maxY - radius;
	thisVertices = vertices;
	for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
	{
		thisPolygonPt->x = tagCenter.x - thisVertices->x;
		thisPolygonPt->y = tagCenter.y + thisVertices->y;
		// log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
		++aa;
	}
	//���Ͻ�
	tagCenter.x = maxX - radius;
	tagCenter.y = maxY - radius;
	thisVertices = vertices + segments;
	for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
	{
		thisPolygonPt->x = tagCenter.x + thisVertices->x;
		thisPolygonPt->y = tagCenter.y + thisVertices->y;
		// log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
		++aa;
	}
	//���½�
	tagCenter.x = maxX - radius;
	tagCenter.y = minY + radius;
	thisVertices = vertices;
	for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
	{
		thisPolygonPt->x = tagCenter.x + thisVertices->x;
		thisPolygonPt->y = tagCenter.y - thisVertices->y;
		// log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
		++aa;
	}
	//���½�
	tagCenter.x = minX + radius;
	tagCenter.y = minY + radius;
	thisVertices = vertices + segments;
	for (unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
	{
		thisPolygonPt->x = tagCenter.x - thisVertices->x;
		thisPolygonPt->y = tagCenter.y - thisVertices->y;
		// log("%f , %f", thisPolygonPt->x, thisPolygonPt->y);
		++aa;
	}

	//���ò���
	static Color4F red(1, 0, 0, 1);//������ɫ����Ϊ��ɫ��������R,G,B,͸����

	//ע�ⲻҪ��pStencil addChild
	DrawNode *pStencil = DrawNode::create();
	pStencil->drawPolygon(pPolygonPtArr, dwPolygonPtMax, red, 0, red);//������������
	pStencil->setPosition(Point(0, 0));
	pClip->setStencil(pStencil);

	pClip->addChild(thisbgSprite, 1, 1);
	pClip->setContentSize(thisbgSprite->getContentSize());

	CC_SAFE_DELETE_ARRAY(vertices);
	CC_SAFE_DELETE_ARRAY(pPolygonPtArr);

	return pClip;
}

// �ص�����
int callback(void* para, int col_num, char** col_value, char** col_name)
{
	log("%s : total %d parm", (char *)para, col_num);
	for (int i = 0; i < col_num; i++) {
		log("%s = %s", col_name[i], col_value[i]);
	}
	log("------------------------------"); // �ָ���

	return 0;
}

bool createSqlite3()
{
	/**
	1.�������ݿ� ���ļ������ڣ�sqlite���Զ����������ļ����ڣ��ͳ��԰����������ݿ��ļ�����
	*/
	std::string path = FileUtils::getInstance()->fullPathForFilename("data.db");	// ���ݿ�·��
	std::string sql;									// sql���
	int ret;											// ִ�н�� SQLITE_OK��ʾ�ɹ�ִ��

	// �������ݿ�sqlite3*
	sqlite3* pdb = nullptr;
	ret = sqlite3_open(path.c_str(), &pdb);
	// �����ݿ��ʧ��ʱ
	if (ret != SQLITE_OK){
		// ������ݿ�򿪴������Ϣ
		const char* errmsg = sqlite3_errmsg(pdb);
		log("sqlite open error:%d", errmsg);
		sqlite3_close(pdb);
		return false;
	}
	/**
	2.�򿪱�
	����һ�ű�create table ����(ID , name , sex)
	������student �� �������ֶΣ�ID��name��sex  ������IDΪ������������
	PS������������Ҫ�ص�������sqlite3_exec �ĺ���������������NULL��
	*/
	// �������sql���
	sql = "create table student(ID integer primary key autoincrement, name text, sex text)";
	// ִ��sql��䣺sqlite3_exec
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK){
		log("create table failed");
	}
	/**
	3.�������ݣ�����
	�����������ݣ�insert into ���� value(1 , 'name' , 'sex')
	PS���������ݣ�����Ҫ�ص�������sqlite3_exec �ĺ��������������� NULL ��
	*/
	// (1 , 'student1' , 'male')
	sql = "insert into student values(1, 'student1', 'male')";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("insert data failed!");
	}
	// (2 , 'student3' , 'female')
	sql = "insert into student values(2, 'student2', 'female')";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("insert data failed!");
	}

	// (3 , 'student3' , 'male')
	sql = "insert into student values(3, 'student3', 'male')";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("insert data failed!");
	}
	/**
	4.��ѯ���ݣ��飩
	ʹ�� sqlite3_exec ����ѯ��select * from ����
	*/
	// ��ѯ���
	sql = "select * from student";
	ret = sqlite3_exec(pdb, sql.c_str(), &callback, (void*)"para", nullptr);
	/**
	5.ɾ�����ݣ�ɾ��
	ɾ����һ����¼��delete from ���� where ����
	*/
	// ɾ����һ����¼
	sql = "delete from student where ID = 1";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("delete data failed!");
	}

	/**
	6.�޸����ݣ��ģ�
	�޸����ݣ�update ���� set name = "hello" where ����
	*/
	// �޸ĵ�������¼�� name �ֶ�
	sql = "update student set name = 'hello' where ID = 3";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("update data failed!");
	}

	/**
	7.��ѯ���ݣ�ʹ��sqlite3_get_table���飩
	ʹ�� sqlite3_get_table ����ѯ��select * from ����
	��ѯ��󣬲�ʹ�ûص�����������ֱ�ӱ����ѯ�����
	*/
	char** table; // ��ѯ���
	int r, c;     // ����������

	sql = "select * from student";
	sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);

	log("row is %d , col is %d", r, c);

	// ��0�У�0 ~ c-1����Ϊ�ֶ���
	// ��1�У�c ~ 2*c-1������һ����¼
	// ......
	for (int i = 0; i <= r; i++) {
		for (int j = 0; j < c; j++) {
			log("%s", table[i * c + j]);
		}
		log("------------------------------");
	}
	// �ǵ��Ƿ��ѯ��
	sqlite3_free_table(table);

	/**
	8.�ر����ݿ� ���������ڴ�й¶
	*/
	sqlite3_close(pdb);

	return true;
}

//��ȡ��ǰʱ��
time_t getSecondTime(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	struct timeval now;

	gettimeofday(&now, NULL);
	time_t time_sec;
	time_sec = now.tv_sec;

	return time_sec;

#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

	time_t timep;
	time(&timep);

	return timep;

#endif
}

std::string getCurrentTime()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	struct timeval now;
	struct tm* tm;

	gettimeofday(&now, NULL);
	time_t time_sec;
	time_sec = now.tv_sec;
	tm = localtime(&time_sec);

	int year = tm->tm_year + 1900;
	log("year = %d", year);

	char date[32] = { 0 };
	sprintf(date, "%d-%02d-%02d", (int)tm->tm_year + 1900, (int)tm->tm_mon + 1, (int)tm->tm_mday);
	log("%s", date);
	char nowTime[32] = { 0 };
	sprintf(nowTime, "%02d:%02d:%02d", (int)tm->tm_hour, (int)tm->tm_min, (int)tm->tm_sec);
	log("%s", nowTime);
	return StringUtils::format("%s", nowTime);

#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

	struct tm* tm;
	time_t timep;
	time(&timep);

	tm = localtime(&timep); //auto ssss = asctime(tm);//Wed Jan 07 16:55:53 2015\n
	char date[32] = { 0 };
	sprintf(date, "%d-%02d-%02d", (int)tm->tm_year + 1900, (int)tm->tm_mon + 1, (int)tm->tm_mday);
	log("%s", date);
	char nowTime[32] = { 0 };
	sprintf(nowTime, "%02d:%02d:%02d", (int)tm->tm_hour, (int)tm->tm_min, (int)tm->tm_sec);
	log("%s", nowTime);
	return StringUtils::format("%s", nowTime);

#endif
}

//�ַ����ָ�� src--���ָ��Դ�ַ��� delim--�ָ��ַ�
std::vector<std::string> split(const std::string& src, const std::string& delim)
{
	std::vector<std::string> elems;
	size_t pos = 0;
	size_t len = src.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = src.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(src.substr(pos, len - pos));
			break;
		}
		elems.push_back(src.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}