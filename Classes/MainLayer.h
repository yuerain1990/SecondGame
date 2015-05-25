#pragma once
#include "cocos2d.h"
#include "TopLayer.h"
USING_NS_CC;

class MainLayer : public Layer
{
public:
	MainLayer();
	~MainLayer();
	
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MainLayer);

	virtual void onEnter() override;
	virtual void onExit() override;
	void tapBarMenuPressed(Ref* pSender);

private:
	LayerMultiplex* layers;
	Menu* tabBarItems;
	Sprite* tabBarBg;
	int numMes;
	int numDia;

	//TopLayer* topLayer;
	CC_SYNTHESIZE_READONLY(TopLayer*, m_topLayer, TopLayer);
};
