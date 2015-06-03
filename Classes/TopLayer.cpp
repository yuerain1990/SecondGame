#include "TopLayer.h"
#include "Chinese.h"
#include "FilterSprite.h"
#include "ShaderEffect.h"

TopLayer::TopLayer() : numMes(0), numDia(0)
{
}

TopLayer::~TopLayer()
{
	Chinese::getInstance()->destoryInstance();
}

bool TopLayer::init(){
	if (!Layer::init()){
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//信息层背景
	//auto topbar = FilterSprite::createWithSpriteFrameName("hengtiao.png");
	auto topbar = EffectSprite::createWithSpriteFrameName("hengtiao.png");
	topbar->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height - topbar->getContentSize().height * 0.5));
	this->addChild(topbar);
	//topbar->setFilterMat(FilterSprite::ES_COLD);
	topbar->setEffect(EffectGrey::create());

	//头标题
	/*auto */info = Label::createWithSystemFont(Chinese::getInstance()->ChineseWord("message"), "Arial", 40);
	info->setPosition(Vec2(visibleSize.width * 0.5, 50));
	//info->setColor(Color3B::BLACK);
	topbar->addChild(info);

	//添加信息
	auto mes = Sprite::createWithSpriteFrameName("youjian.png");
	mes->setPosition(Vec2(30, topbar->getContentSize().height - 30));
	topbar->addChild(mes);
	//+按钮
	auto menuMes = MenuItemSprite::create(Sprite::createWithSpriteFrameName("shuxiangchangtiao.png"), Sprite::createWithSpriteFrameName("shuxiangchangtiao.png"), CC_CALLBACK_1(TopLayer::menuCallback, this));
	menuMes->setTag(1);
	auto menu1 = Menu::create(menuMes, nullptr);
	menu1->setPosition(Vec2(155, topbar->getContentSize().height - 30));
	topbar->addChild(menu1);
	//数值
	auto labelMes = Label::createWithTTF("0/0", "fonts/arial.ttf", 30);
	labelMes->setPosition(menuMes->getContentSize() * 0.5);
	labelMes->setTag(10);
	menuMes->addChild(labelMes);

	//添加钻石
	auto dia = Sprite::createWithSpriteFrameName("zhuanshi.png");
	dia->setPosition(Vec2(topbar->getContentSize().width - 235, topbar->getContentSize().height - 30));
	topbar->addChild(dia);
	//+按钮 normalSprite selectedSprite disabledSprite这些添加到MenuItemSprite上的 已经占用了tag值 分别为1 2 3 
	auto menuDia = MenuItemSprite::create(Sprite::createWithSpriteFrameName("shuxiangchangtiao.png"), Sprite::createWithSpriteFrameName("shuxiangchangtiao.png"), CC_CALLBACK_1(TopLayer::menuCallback, this));
	menuDia->setTag(2);
	auto menu2 = Menu::create(menuDia, nullptr);
	menu2->setPosition(Vec2(topbar->getContentSize().width - 110, topbar->getContentSize().height - 30));
	topbar->addChild(menu2);
	//数值
	auto labelDia = Label::createWithTTF("0", "fonts/arial.ttf", 30);
	labelDia->setPosition(menuDia->getContentSize() * 0.5);
	labelDia->setTag(10);
	menuDia->addChild(labelDia);

	return true;
}

void TopLayer::menuCallback(Ref* pSender){
	auto tmpItem = static_cast<MenuItemSprite*>(pSender);
	switch (tmpItem->getTag())
	{
		case 1:
		{
			log("tag 1 clicked");
			auto label = static_cast<Label*>(tmpItem->getChildByTag(10));
			label->setString(StringUtils::format("%d/100", ++numMes));
		}
			break;
		case 2:
		{
			log("tag 2 clicked");
			auto label = static_cast<Label*>(tmpItem->getChildByTag(10));
			label->setString(StringUtils::format("%d", ++numDia));
		}
			break;
	}
}

void TopLayer::onEnter(){
	Layer::onEnter();
	info->setString("message");
}

void TopLayer::onExit(){
	Layer::onExit();
}

void TopLayer::alterInfoWithIndex(int index){
	switch (index)
	{
		case 0:
		{
			info->setString("message");
		}
			break;
		case 1:
		{
			info->setString("friend");
		}
			break;
		case 2:
		{
			info->setString("discover");
		}
			break;
		case 3:
		{
			info->setString("set");
		}
			break;
		default:
			break;
	}
}
