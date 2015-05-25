#include "FunctionUtils.h"

/**
*画圆角矩形 是在底板的基础上画 故开始点 和 结束点是底板的左下角和右上角 底板的锚点是在中心中心点坐标为(0, 0)
*bgSprite底板 origin矩形开始点 destination矩形结束点 radius半径 segments圆角等份数，等分越多，圆角越平滑
*/
ClippingNode* ccDrawRoundRect(Sprite *bgSprite, Vec2 origin, Vec2 destination, float radius, unsigned int segments)
{
	Sprite *thisbgSprite = bgSprite;
	ClippingNode* pClip = ClippingNode::create();

	pClip->setInverted(false);//设置是否反向，将决定画出来的圆是透明的还是黑色的
	//    this->addChild(pClip);
	pClip->setAnchorPoint(Point(0, 0));
	pClip->setPosition(-7, -7);

	//算出1/4圆
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
	//左上角
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
	//右上角
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
	//右下角
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
	//左下角
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

	//设置参数
	static Color4F red(1, 0, 0, 1);//顶点颜色设置为红色，参数是R,G,B,透明度

	//注意不要将pStencil addChild
	DrawNode *pStencil = DrawNode::create();
	pStencil->drawPolygon(pPolygonPtArr, dwPolygonPtMax, red, 0, red);//绘制这个多边形
	pStencil->setPosition(Point(0, 0));
	pClip->setStencil(pStencil);

	pClip->addChild(thisbgSprite, 1, 1);
	pClip->setContentSize(thisbgSprite->getContentSize());

	CC_SAFE_DELETE_ARRAY(vertices);
	CC_SAFE_DELETE_ARRAY(pPolygonPtArr);

	return pClip;
}

// 回调函数
int callback(void* para, int col_num, char** col_value, char** col_name)
{
	log("%s : total %d parm", (char *)para, col_num);
	for (int i = 0; i < col_num; i++) {
		log("%s = %s", col_name[i], col_value[i]);
	}
	log("------------------------------"); // 分隔符

	return 0;
}

bool createSqlite3()
{
	/**
	1.创建数据库 若文件不存在，sqlite会自动建立。若文件存在，就尝试把它当做数据库文件来打开
	*/
	std::string path = FileUtils::getInstance()->fullPathForFilename("data.db");	// 数据库路径
	std::string sql;									// sql语句
	int ret;											// 执行结果 SQLITE_OK表示成功执行

	// 创建数据库sqlite3*
	sqlite3* pdb = nullptr;
	ret = sqlite3_open(path.c_str(), &pdb);
	// 当数据库打开失败时
	if (ret != SQLITE_OK){
		// 获得数据库打开错误的信息
		const char* errmsg = sqlite3_errmsg(pdb);
		log("sqlite open error:%d", errmsg);
		sqlite3_close(pdb);
		return false;
	}
	/**
	2.打开表
	创建一张表：create table 表名(ID , name , sex)
	表名：student 。 有三个字段：ID、name、sex  （其中ID为主键自增）。
	PS：创建表，不需要回调函数。sqlite3_exec 的后面三个参数都填NULL。
	*/
	// 创建表的sql语句
	sql = "create table student(ID integer primary key autoincrement, name text, sex text)";
	// 执行sql语句：sqlite3_exec
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK){
		log("create table failed");
	}
	/**
	3.插入数据（增）
	插入三条数据：insert into 表名 value(1 , 'name' , 'sex')
	PS：插入数据，不需要回调函数。sqlite3_exec 的后面三个参数都填 NULL 。
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
	4.查询数据（查）
	使用 sqlite3_exec 来查询：select * from 表名
	*/
	// 查询语句
	sql = "select * from student";
	ret = sqlite3_exec(pdb, sql.c_str(), &callback, (void*)"para", nullptr);
	/**
	5.删除数据（删）
	删除第一条记录：delete from 表名 where 条件
	*/
	// 删除第一条记录
	sql = "delete from student where ID = 1";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("delete data failed!");
	}

	/**
	6.修改数据（改）
	修改数据：update 表名 set name = "hello" where 条件
	*/
	// 修改第三条记录的 name 字段
	sql = "update student set name = 'hello' where ID = 3";
	ret = sqlite3_exec(pdb, sql.c_str(), nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) {
		log("update data failed!");
	}

	/**
	7.查询数据，使用sqlite3_get_table（查）
	使用 sqlite3_get_table 来查询：select * from 表名
	查询完后，不使用回调函数，而是直接保存查询结果。
	*/
	char** table; // 查询结果
	int r, c;     // 行数、列数

	sql = "select * from student";
	sqlite3_get_table(pdb, sql.c_str(), &table, &r, &c, nullptr);

	log("row is %d , col is %d", r, c);

	// 第0行（0 ~ c-1），为字段名
	// 第1行（c ~ 2*c-1），第一条记录
	// ......
	for (int i = 0; i <= r; i++) {
		for (int j = 0; j < c; j++) {
			log("%s", table[i * c + j]);
		}
		log("------------------------------");
	}
	// 记得是否查询表
	sqlite3_free_table(table);

	/**
	8.关闭数据库 否则会造成内存泄露
	*/
	sqlite3_close(pdb);

	return true;
}

//获取当前时间
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

//字符串分割函数 src--待分割的源字符串 delim--分割字符
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