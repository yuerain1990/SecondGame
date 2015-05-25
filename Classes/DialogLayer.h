#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIScrollView.h"
#include "ui/UIListView.h"
#include "ui/UIImageView.h"
USING_NS_CC;
USING_NS_CC_EXT;

class TopLayer;
class BottomLayer;
class DBUtil;

struct Dialog
{
	char id[1000];
	char boy_id[1000];
	char boy_word[1000];
	char select_word[1000];
	char key_word_id[1000];
};

struct DataBoy
{
	char boy_id[1000];
	char score[1000];
	char current_word_id[1000];
	char you_word[1000];
	char me_word[1000];
};

class DialogLayer : public LayerColor
{
public:
	DialogLayer();
	~DialogLayer();

	static Scene* createSceneWithIndex(int index);
	static DialogLayer* createWithIndex(int index);
	bool initWithIndex(int index);

	ui::Scale9Sprite* youWordWithId(int id, int score, std::string meword);
	ui::Scale9Sprite* meWordWithId(std::string str);
	//换行
	std::string myWrap(std::string str, int length);

	//ListView
	void createListView();
	void selectedItemEvent(Ref* pSender, ui::ListView::EventType type);
	void insertYouItemWithId(int id, int score, std::string meword);
	void insertMeItemWithId(std::string str);


	virtual void onEnter() override;
	virtual void onExit() override;

	/**
	 *获取ListView
	 */
	ui::ListView* getListView();

	CC_SYNTHESIZE_READONLY(TopLayer*, _topLayer, TopLayer);
	CC_SYNTHESIZE_READONLY(BottomLayer*, _bottomLayer, BottomLayer);
	/**
	 *菜单按钮回调函数
	 */
	void menuCallback(Ref* pSender);
	/**修改好感度分数
	 *@score	分数
	 *@v		传入的分数变化+5还是-5
	 *@color	传入的颜色绿色还是红色
	 */
	void alterHeartScore(float score, const char* v, Color3B& color);
	/**查询表game_data的内容
	 *@index	索引id
	 */
	struct DataBoy* searchGameData(int index);
	/**查询表game_data的内容
     *@pid		索引id
	 *@pboyid	对象id
	 */
	struct Dialog* searchDialog(int pid, int pboyid);
	/**
	 *设置ListView的内容
	 */
	void setListViewContent();
	/**对象话语内容 用于下次进入显示已经对话的内容
	 *@pStr		内容 
	 *@pTime	时间
	 */
	void youWordWithContent(std::string pStr, std::string pTime);
	/**我的话语内容 用于下次进入显示已经对话的内容
	*@pStr		内容
	*@pTime		时间
	*/
	void meWordWithContent(std::string pStr, std::string pTime);
	CC_SYNTHESIZE(int, _no, No);
	CC_SYNTHESIZE(float, _score, Score);
private:
	int _index;
	Size winSize;
	ui::ListView* _listView;
	//int _no;
	ProgressTimer* _heart;
};

