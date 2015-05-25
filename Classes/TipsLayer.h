#pragma once
#include "cocos2d.h"
USING_NS_CC;

class TipsLayer : public LayerColor
{
public:
	TipsLayer();
	~TipsLayer();

	virtual bool init();
	CREATE_FUNC(TipsLayer);

	bool onTouchBegan(Touch* touch, Event* event);//触摸开始方法
	void onTouchMoved(Touch* touch, Event* event);//触摸移动方法
	void onTouchEnded(Touch* touch, Event* event);//触摸结束方法

	void menuCallback(Ref* sender);
private:

};

