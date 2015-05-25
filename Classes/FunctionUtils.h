#include "cocos2d.h"
#include "sqlite3.h"
USING_NS_CC;

ClippingNode* ccDrawRoundRect(Sprite *bgSprite, Vec2 origin, Vec2 destination, float radius, unsigned int segments);

int callback(void* para, int col_num, char** col_value, char** col_name);

bool createSqlite3();

std::string getCurrentTime();

time_t getSecondTime();

#define PERSON_EVENT "person event"

//×Ö·û´®·Ö¸îº¯Êý src--´ý·Ö¸îµÄÔ´×Ö·û´® delim--·Ö¸î×Ö·û
std::vector<std::string> split(const std::string& src, const std::string& delim);
