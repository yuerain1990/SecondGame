#include "TipsLayer.h"

TipsLayer::TipsLayer()
{
}

TipsLayer::~TipsLayer()
{
}

bool TipsLayer::init(){
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 150))){
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();
	//����
	auto tip = Sprite::create("tip.png");
	tip->setPosition(winSize * 0.5);
	this->addChild(tip);
	//��ʾ��
	auto label = Label::createWithSystemFont("You should get the right answer.", "Arial", 25);
	label->setPosition(tip->getContentSize() * 0.5);
	tip->addChild(label);
	//��ť
	auto menuLabel = MenuItemLabel::create(Label::createWithSystemFont("CLOSE", "Arial", 40), CC_CALLBACK_1(TipsLayer::menuCallback, this));
	auto menu = Menu::create(menuLabel, nullptr);
	menu->setPosition(Vec2(tip->getContentSize().width - 80, 50));
	tip->addChild(menu);

	//������Ļ�����¼�
	auto listener = EventListenerTouchOneByOne::create();
	//���ÿ����´���
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(TipsLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TipsLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TipsLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool TipsLayer::onTouchBegan(Touch* touch, Event* event){

	return true;
}

void TipsLayer::onTouchMoved(Touch* touch, Event* event){

}

void TipsLayer::onTouchEnded(Touch* touch, Event* event){

}

void TipsLayer::menuCallback(Ref* sender){
	auto menuLabel = static_cast<MenuItemLabel*>(sender);
	this->removeFromParent();
}