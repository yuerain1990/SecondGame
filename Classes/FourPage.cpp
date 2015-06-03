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
*信息
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

	//ScrollView上添加单行信息
	//设置容器
	Layer* containerLayer = Layer::create();
	containerLayer->setContentSize(Size(640, GIRL_NUM * 132));
	containerLayer->setPosition(Vec2(0, 0));
	containerLayer->setAnchorPoint(Vec2::ZERO);
	//添加单行信息 每行信息的i值不一定是自增的 可能是乱的 这里需要修改*** 
	//思路：需要将当前每行对象的编号id存储到数据库中，每次加载的时候根据id展示每行数据，
	//取出来的id应该是一串字符串，需要进行字符串分割，保存在一个向量中。最后遍历这个向量，取出其中的id值
	//GIRL_NUM是所有id的总和
	/*数据库中存储的vecid
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
	//创建ScrollView
	ScrollView* scrollView = ScrollView::create();
	scrollView->setViewSize(Size(640, visibleSize.height - 130 - 80));
	scrollView->setPosition(Vec2(0, 80));
	scrollView->setAnchorPoint(Vec2(0, 0));
	scrollView->setContainer(containerLayer);
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setDelegate(this);
	this->addChild(scrollView, 1);
	scrollView->setContentOffset(Vec2(0, -GIRL_NUM * 132 + visibleSize.height - 130 - 80));
	
	//创建屏幕监听事件
	auto listener = EventListenerTouchOneByOne::create();
	//设置可向下传递
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
*密友
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
*发现
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
*设置
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