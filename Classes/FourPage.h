#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

/**
 *信息
 */
class MessagePage : public LayerColor, public ScrollViewDelegate
{
public:
	MessagePage();
	~MessagePage();

	virtual bool init();
	CREATE_FUNC(MessagePage);

	void menuCallback(Ref* sender);

	bool onTouchBegan(Touch* touch, Event* event);//触摸开始方法
	void onTouchMoved(Touch* touch, Event* event);//触摸移动方法
	void onTouchEnded(Touch* touch, Event* event);//触摸结束方法

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
private:
	Vector<LayerColor*> rowInfoVec;
	Vector<MenuItem*> menuSpriteVec;
};

/**
*密友
*/
class FriendPage : public LayerColor
{
public:
	FriendPage();
	~FriendPage();

	virtual bool init();
	CREATE_FUNC(FriendPage);
private:

};

/**
*发现
*/
class DiscoverPage : public LayerColor
{
public:
	DiscoverPage();
	~DiscoverPage();

	virtual bool init();
	CREATE_FUNC(DiscoverPage);
private:

};

/**
*设置
*/
class SetPage : public LayerColor
{
public:
	SetPage();
	~SetPage();

	virtual bool init();
	CREATE_FUNC(SetPage);
private:

};

