#include "Chinese.h"

//static Chinese* instance = nullptr;
Chinese* Chinese::instance = nullptr;

Chinese::Chinese()
{
	txt_map = FileUtils::getInstance()->getValueMapFromFile("fonts/strings.xml");
}

Chinese::~Chinese()
{
	
}

Chinese* Chinese::getInstance(){
	if (!instance){
		instance = new Chinese();
	}
	return instance;
}

void Chinese::destoryInstance(){
	CC_SAFE_DELETE(instance);
}

std::string Chinese::ChineseWord(const char* wordKey){    
	std::string ret = txt_map.at(wordKey).asString();
	return ret;
}