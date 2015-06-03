//	Created by yuerain
#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class SelectPerson : public LayerColor
{
public:
	SelectPerson();
	~SelectPerson();

	CREATE_FUNC(SelectPerson);
	virtual bool init();

	void updateData(float dt);
	void setTipValue(int count);
	void discussResult(float dt);
private:
	Vector<ui::Widget*> rowVec;
	ui::Text* _tip;
	ui::Button* _button;
};

