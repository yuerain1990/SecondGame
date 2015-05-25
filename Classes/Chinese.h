#ifndef _CHINESE_H_
#define _CHINESE_H_

#include <string>
#include <cocos2d.h>
using namespace std;
using namespace cocos2d;

class Chinese
{
public:
	Chinese();
	~Chinese();

	static Chinese* getInstance();
	static void destoryInstance();	//在函数结尾调用销毁函数
	ValueMap txt_map;

	std::string ChineseWord(const char* wordKey);
private:
	static Chinese* instance;
};


#endif