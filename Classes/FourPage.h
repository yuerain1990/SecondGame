#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

/**
 *��Ϣ
 */
class MessagePage : public LayerColor, public ScrollViewDelegate
{
public:
	MessagePage();
	~MessagePage();

	virtual bool init();
	CREATE_FUNC(MessagePage);

	void menuCallback(Ref* sender);

	bool onTouchBegan(Touch* touch, Event* event);//������ʼ����
	void onTouchMoved(Touch* touch, Event* event);//�����ƶ�����
	void onTouchEnded(Touch* touch, Event* event);//������������

	virtual void scrollViewDidScroll(ScrollView* view);
	virtual void scrollViewDidZoom(ScrollView* view);
private:
	Vector<LayerColor*> rowInfoVec;
	Vector<MenuItem*> menuSpriteVec;
};

/**
*����
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
*����
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
*����
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

