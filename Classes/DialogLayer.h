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
	//����
	std::string myWrap(std::string str, int length);

	//ListView
	void createListView();
	void selectedItemEvent(Ref* pSender, ui::ListView::EventType type);
	void insertYouItemWithId(int id, int score, std::string meword);
	void insertMeItemWithId(std::string str);


	virtual void onEnter() override;
	virtual void onExit() override;

	/**
	 *��ȡListView
	 */
	ui::ListView* getListView();

	CC_SYNTHESIZE_READONLY(TopLayer*, _topLayer, TopLayer);
	CC_SYNTHESIZE_READONLY(BottomLayer*, _bottomLayer, BottomLayer);
	/**
	 *�˵���ť�ص�����
	 */
	void menuCallback(Ref* pSender);
	/**�޸ĺøжȷ���
	 *@score	����
	 *@v		����ķ����仯+5����-5
	 *@color	�������ɫ��ɫ���Ǻ�ɫ
	 */
	void alterHeartScore(float score, const char* v, Color3B& color);
	/**��ѯ��game_data������
	 *@index	����id
	 */
	struct DataBoy* searchGameData(int index);
	/**��ѯ��game_data������
     *@pid		����id
	 *@pboyid	����id
	 */
	struct Dialog* searchDialog(int pid, int pboyid);
	/**
	 *����ListView������
	 */
	void setListViewContent();
	/**���������� �����´ν�����ʾ�Ѿ��Ի�������
	 *@pStr		���� 
	 *@pTime	ʱ��
	 */
	void youWordWithContent(std::string pStr, std::string pTime);
	/**�ҵĻ������� �����´ν�����ʾ�Ѿ��Ի�������
	*@pStr		����
	*@pTime		ʱ��
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

