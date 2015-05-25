#include "RowInfo.h"
#include "FunctionUtils.h"
#include "Chinese.h"
#include "DBMenu.h"
#include "DialogLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
#endif

RowInfo::RowInfo()
{

}

RowInfo::~RowInfo()
{
	Chinese::getInstance()->destoryInstance();	//����ģʽ�ͷ��ڴ�
}

RowInfo* RowInfo::createWithHeadName(const std::string& headName, int index){
	auto ret = new RowInfo();
	if (ret && ret->initWithHeadName(headName, index)){
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool RowInfo::initWithHeadName(const std::string& headName, int index){
	if (!LayerColor::initWithColor(Color4B::RED, 640, 130)){
		return false;
	}
	/*headcell*/
	auto headCell = Sprite::createWithSpriteFrameName("headcell.png");
	headCell->setPosition(Vec2::ZERO);
	headCell->setAnchorPoint(Vec2::ZERO);
	auto headSprite = Sprite::createWithSpriteFrameName(headName);
	ClippingNode* headNode = ccDrawRoundRect(headSprite, Vec2(-50, -50), Vec2(50, 50), 15, 1000);
	headNode->setPosition(Vec2(headSprite->getContentSize().width * 0.5 + 15, headSprite->getContentSize().height * 0.5 + 15));
	headCell->addChild(headNode);
	auto menuItem = MenuItemSprite::create(headCell, nullptr, CC_CALLBACK_1(RowInfo::menuCallbackHead, this));
	menuItem->setPosition(Vec2(0, 0));
	menuItem->setAnchorPoint(Vec2::ZERO);
	menuItem->setTag(index);	//����Ӧ���Ǵ���Ϣҳ�洫������idֵ
	auto menu = DBMenu::create(menuItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	menu->setSwallowsTouches(false);
	menu->setCheckScissor(true);//���menuitem���������ⱻ���������� ֻ�е�menu����scrollview�ϲ�����Ϊtrue
	this->addChild(menu);

	/*infocell*/
	auto infoCell = Sprite::createWithSpriteFrameName("infocell.png");
	infoCell->setPosition(Vec2(0, 0));
	infoCell->setAnchorPoint(Vec2::ZERO);
	//this->addChild(infoCell);
	//����
	auto nameLabel = Label::createWithSystemFont(Chinese::getInstance()->ChineseWord("one"), "Arial", 30);
	nameLabel->setPosition(Vec2(30, headSprite->getContentSize().height - 10));
	nameLabel->setColor(Color3B::BLACK);
	nameLabel->setAnchorPoint(Vec2::ZERO);
	infoCell->addChild(nameLabel);
	//�Ա�����
	auto sex = Sprite::create("sex.png");
	sex->setPosition(Vec2(30, 50));
	sex->setAnchorPoint(Vec2::ZERO);
	infoCell->addChild(sex);
	auto ageLabel = Label::createWithSystemFont("25", "Arial", 25);  
	ageLabel->setPosition(Vec2(sex->getContentSize().width - 20, sex->getContentSize().height * 0.5));
	sex->addChild(ageLabel);
	//����һ����Ϣ ����Ҫ��onEnter����������ֵ�� Ҫ�ڱ�game_data��ȡ�����һ�����ݳ���
	auto info = Label::createWithSystemFont(Chinese::getInstance()->ChineseWord("info"), "Arial", 25);
	info->setPosition(Vec2(30, 10));
	info->setColor(Color3B(166, 166, 199));
	info->setAnchorPoint(Vec2::ZERO); 
	infoCell->addChild(info);
	//�������ʱ��
	auto timeLabel = Label::createWithSystemFont("18:59", "Arial", 20);
	timeLabel->setPosition(Vec2(infoCell->getContentSize().width - 60, this->getContentSize().height - 30));
	timeLabel->setColor(Color3B::BLACK);
	infoCell->addChild(timeLabel);

	auto menuItemInfo = MenuItemSprite::create(infoCell, nullptr, CC_CALLBACK_1(RowInfo::menuCallbackInfo, this));
	menuItemInfo->setPosition(Vec2::ZERO);
	menuItemInfo->setAnchorPoint(Vec2::ZERO);
	menuItemInfo->setTag(index);
	auto menuInfo = DBMenu::create(menuItemInfo, nullptr);
	menuInfo->setPosition(Vec2(130, 0));
	menuInfo->setSwallowsTouches(false);
	menuInfo->setCheckScissor(true);//���menuitem���������ⱻ���������� ֻ�е�menu����scrollview�ϲ�����Ϊtrue
	this->addChild(menuInfo);

	//����
	auto rowLabel = Label::createWithTTF(StringUtils::format("%d", index), "fonts/arial.ttf", 30);
	rowLabel->setPosition(Vec2(15, 15));
	rowLabel->setColor(Color3B::BLUE);
	this->addChild(rowLabel);


	return true;
}
/**
 *ͷ�񱻵��
 */
void RowInfo::menuCallbackHead(Ref* sender){
	auto menuItem = static_cast<MenuItem*>(sender);
	log("---%d", menuItem->getTag());

	auto girl = Sprite::createWithSpriteFrameName(StringUtils::format("girl%03d.png", menuItem->getTag()));
	girl->setScale(5.0);
	girl->setPosition(Vec2(320, 568));
	girl->getTexture()->setAntiAliasTexParameters();
	//girl->getTexture()->setAliasTexParameters();
	this->getParent()->getParent()->getParent()->addChild(girl, 100);
}

void RowInfo::menuCallbackInfo(Ref* sender){
	auto menuItem = static_cast<MenuItem*>(sender);
	log("===%d", menuItem->getTag());

	//������Ҫ��tag���뵽DialogLayer�� ��ô���������ݵ��� ��Ϊ��������ȥ
	

	//ʾ������һ��menu��ʵ������ ���ʱ ����һ���¼��ַ� Ȼ������label��ֵ
	//EventCustom
	/*char* buf = new char[10];
	sprintf(buf, "%d", menuItem->getTag());
	EventCustom event(PERSON_EVENT);
	event.setUserData(buf);
	_eventDispatcher->dispatchEvent(&event);
	CC_SAFE_DELETE_ARRAY(buf);*/

	Director::getInstance()->pushScene(TransitionSlideInR::create(0.2f, DialogLayer::createSceneWithIndex(menuItem->getTag())));
	//this->getParent()->getParent()->getParent()->addChild(DialogLayer::create(), 100);

}

/**
 ����Ե���Ҫ�ŵ���һλ�����ν������У��������Ҫ�õ�����
 ���ļ�����Ҫ�������������˵�����index�����˵�Ļ�lastword�����˵����ʱ��time ������
 **/

void RowInfo::onEnter(){
	LayerColor::onEnter();

}

void RowInfo::onExit(){
	LayerColor::onExit();

}