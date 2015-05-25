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

	void menuCallback(Ref* sender);									//菜单回调
	void listViewSizeChange(ui::ListView* listView, int factor);	//listview size修改
	void resetLabelValueWithId(int id);								//重新设置标签的值
	void destoryMemory();											//销毁内存
	void sendMessage(MenuItemSprite* menuSprite, DialogLayer* dialogLayer, ui::ListView* listView, MoveBy* moveBy);
private:
	Sprite* _bottom_bg;			//底部背景
	MenuItemSprite* _popItem;	//弹条按钮
	int _no;					//当前是第几条对话内容
	Label* _firstLabel;			//第一条
	Label* _secondLabel;		//第二条
	float _score;				//好感度分数
};
