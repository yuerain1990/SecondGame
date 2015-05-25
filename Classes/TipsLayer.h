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

	bool onTouchBegan(Touch* touch, Event* event);//������ʼ����
	void onTouchMoved(Touch* touch, Event* event);//�����ƶ�����
	void onTouchEnded(Touch* touch, Event* event);//������������

	void menuCallback(Ref* sender);
private:

};

