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
	static void destoryInstance();	//�ں�����β�������ٺ���
	ValueMap txt_map;

	std::string ChineseWord(const char* wordKey);
private:
	static Chinese* instance;
};


#endif