#include "FourPage.h"
#include "RowInfo.h"
#include "Chinese.h"
#include "SelectPerson.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
#endif

#define GIRL_NUM 11
#define BOY_NUM  11
/**
*��Ϣ
*/
MessagePage::MessagePage()
{
}

MessagePage::~MessagePage()
{
	Chinese::getInstance()->destoryInstance();
}

bool MessagePage::init(){
	if (!LayerColor::initWithColor(Color4B(180, 180, 0, 180))){
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//ScrollView����ӵ�����Ϣ
	//��������
	Layer* containerLayer = Layer::create();
	containerLayer->setContentSize(Size(640, GIRL_NUM * 132));
	containerLayer->setPosition(Vec2(0, 0));
	containerLayer->setAnchorPoint(Vec2::ZERO);
	//��ӵ�����Ϣ ÿ����Ϣ��iֵ��һ���������� �������ҵ� ������Ҫ�޸�*** 
	//˼·����Ҫ����ǰÿ�ж���ı��id�洢�����ݿ��У�ÿ�μ��ص�ʱ�����idչʾÿ�����ݣ�
	//ȡ������idӦ����һ���ַ�������Ҫ�����ַ����ָ������һ�������С����������������ȡ�����е�idֵ
	//GIRL_NUM������id���ܺ�
	/*���ݿ��д洢��vecid
	for(size_t i = 0; i < vecid.size(); i++){
		auto rowInfo = RowInfo::createWithHeadName(StringUtils::format("girl%d03d.png", vecid.at(i)), vecid.at(i));
		rowInfo->setPosition(Vec2(0, (vecid.size() - i - 1) * 132));
		containerLayer->addChild(rowInfo);
	}
	*/
	for (size_t i = 0; i < GIRL_NUM; i++){	
		auto rowInfo = RowInfo::createWithHeadName(StringUtils::format("girl%03d.png", i), i);
		rowInfo->setPosition(Vec2(0, (GIRL_NUM - i - 1) * 132));
		containerLayer->addChild(rowInfo);
	}
	//����ScrollView
	ScrollView* scrollView = ScrollView::create();
	scrollView->setViewSize(Size(640, visibleSize.height - 130 - 80));
	scrollView->setPosition(Vec2(0, 80));
	scrollView->setAnchorPoint(Vec2(0, 0));
	scrollView->setContainer(containerLayer);
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setDelegate(this);
	this->addChild(scrollView, 1);
	scrollView->setContentOffset(Vec2(0, -GIRL_NUM * 132 + visibleSize.height - 130 - 80));
	
	//������Ļ�����¼�
	auto listener = EventListenerTouchOneByOne::create();
	//���ÿ����´���
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(MessagePage::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MessagePage::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MessagePage::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MessagePage::menuCallback(Ref* sender){
	auto menuSprite = static_cast<MenuItemSprite*>(sender);
	log("menuitem tag %d is clicked", menuSprite->getTag());
}

bool MessagePage::onTouchBegan(Touch *touch, Event *unused_event){
	

	return true;
}

void MessagePage::onTouchMoved(Touch *touch, Event *unused_event){

}

void MessagePage::onTouchEnded(Touch *touch, Event *unused_event){
	
}

void MessagePage::scrollViewDidScroll(ScrollView* view){

}

void MessagePage::scrollViewDidZoom(ScrollView* view){

}

/**
*����
*/
FriendPage::FriendPage()
{
}

FriendPage::~FriendPage()
{
}

bool FriendPage::init(){
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 180))){
		return false;
	}

	ui::Button* button = ui::Button::create("closeNormal.png", "closeSelected.png");
	button->setPosition(Vec2(320, 268));
	button->setScale(1.5);
	button->setTitleText("go select person");
	button->setZoomScale(0.3);
	button->setPressedActionEnabled(true);
	button->addClickEventListener([this](Ref* sender){
		this->addChild(SelectPerson::create());
	});
	this->addChild(button);

	return true;
}
/**
*����
*/
DiscoverPage::DiscoverPage()
{
}

DiscoverPage::~DiscoverPage()
{
}

bool DiscoverPage::init(){
	if (!LayerColor::initWithColor(Color4B(0, 180, 180, 180))){
		return false;
	}

	return true;
}

/**
*����
*/
SetPage::SetPage()
{
}

SetPage::~SetPage()
{
}

bool SetPage::init(){
	if (!LayerColor::initWithColor(Color4B(255, 255, 0, 180))){
		return false;
	}

	return true;
}