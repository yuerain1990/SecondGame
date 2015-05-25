#pragma once 
#include "cocos2d.h"

USING_NS_CC;

class TopLayer : public Layer
{
public:
	TopLayer();
	~TopLayer();

	virtual bool init();
	CREATE_FUNC(TopLayer);

	void menuCallback(Ref* pSender);
	virtual void onEnter() override;
	virtual void onExit() override;

	void alterInfoWithIndex(int index);
private:
	int numMes;
	int numDia;
	Label* info;
};
