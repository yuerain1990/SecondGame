#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class RowInfo : public LayerColor
{
public:
	RowInfo();
	~RowInfo();

	static RowInfo* createWithHeadName(const std::string& headName, int index);
	bool initWithHeadName(const std::string& headName, int index);

	void menuCallbackHead(Ref* sender);
	void menuCallbackInfo(Ref* sender);

	virtual void onEnter() override;
	virtual void onExit() override;
private:

};
