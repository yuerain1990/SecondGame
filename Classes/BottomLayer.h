#pragma once 
#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIListView.h"
USING_NS_CC;

class DialogLayer;

class BottomLayer : public Layer
{
public:
	BottomLayer();
	~BottomLayer();

	virtual bool init();
	CREATE_FUNC(BottomLayer);

	virtual void onEnter() override;
	virtual void onExit() override;

	void menuCallback(Ref* sender);									//�˵��ص�
	void listViewSizeChange(ui::ListView* listView, int factor);	//listview size�޸�
	void resetLabelValueWithId(int id);								//�������ñ�ǩ��ֵ
	void destoryMemory();											//�����ڴ�
	void sendMessage(MenuItemSprite* menuSprite, DialogLayer* dialogLayer, ui::ListView* listView, MoveBy* moveBy);
private:
	Sprite* _bottom_bg;			//�ײ�����
	MenuItemSprite* _popItem;	//������ť
	int _no;					//��ǰ�ǵڼ����Ի�����
	Label* _firstLabel;			//��һ��
	Label* _secondLabel;		//�ڶ���
	float _score;				//�øжȷ���
};
