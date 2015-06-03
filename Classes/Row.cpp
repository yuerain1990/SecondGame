//	Created by yuerain
#include "Row.h"

Row::Row()
{
}

Row::~Row()
{
}

Row* Row::createWithName(const std::string& name){
	auto ret = new Row();
	if (ret && ret->initWithName(name)){
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Row::initWithName(const std::string& name){
	if (!Widget::init()){
		return false;
	}

	this->setName("unselected");	//这里需要先设置默认的name 否则的话 假如你没点击的话 name = ""值为空 判断的时候就会出错

	ui::CheckBox* checkBox = ui::CheckBox::create("a81.png", "a82.png");
	checkBox->setPosition(Vec2(40, 0));
	checkBox->setAnchorPoint(Vec2::ZERO);
	checkBox->setName("checkbox");
	checkBox->addEventListener(CC_CALLBACK_2(Row::checkBoxEvent, this));
	this->addChild(checkBox);

	//Text中的数据需要从数据库中的
	/*
	首先 将数据库中的数据取出放到一个容器中vector
	其次 遍历容器 将值填入Text中
	*/
	ui::Text* txt = ui::Text::create();
	txt->setString(name);
	txt->setFontSize(25);
	txt->setPosition(Vec2(100, 10));
	txt->setAnchorPoint(Vec2::ZERO);
	this->addChild(txt);

	return true;
}

void Row::checkBoxEvent(Ref* pSender, ui::CheckBox::EventType type){
	switch (type)
	{
		case cocos2d::ui::CheckBox::EventType::SELECTED:
		{
			log("selected");
			this->setName("selected");
		}
			break;
		case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		{
			log("unselected");
			this->setName("unselected");
		}
			break;
		default:
			break;
	}
}