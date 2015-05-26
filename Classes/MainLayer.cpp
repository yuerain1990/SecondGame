#include "MainLayer.h"
#include "FourPage.h"
#include "FunctionUtils.h"
#include "CSVReader.h"
#include "DBUtil.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
#endif

#define TOOLBARTAG 10086
#define ALL_BOY "all_boy.csv"
#define ALL_GIRL "all_girl.csv"
#define BOY_DIALOG "boy_dialog.csv"
#define GIRL_DIALOG "girl_dialog.csv"
#define IS_FIRST "is_first"

MainLayer::MainLayer() : numMes(0), numDia(0)
{
	DBUtil::getInstance()->openDBWithName("data.db");
}

MainLayer::~MainLayer()
{
	CSVReader::getInstance()->destoryInstance();
	DBUtil::getInstance()->closeDB();
	DBUtil::getInstance()->destoryInstance();
}

Scene* MainLayer::createScene(){
	auto scene = Scene::create();
	auto layer = MainLayer::create();
	scene->addChild(layer);

	return scene;
}

bool MainLayer::init(){
	if (!Layer::init()){
		return false;
	}

	auto layer1 = MessagePage::create();
	auto layer2 = FriendPage::create();
	auto layer3 = DiscoverPage::create();
	auto layer4 = SetPage::create();
	layers = LayerMultiplex::create(layer1, layer2, layer3, layer4, nullptr);
	this->addChild(layers);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//工具条背景
	tabBarBg = Sprite::createWithSpriteFrameName("ditiao.png");
	tabBarBg->setPosition(Vec2(visibleSize.width * 0.5, 0));
	tabBarBg->setAnchorPoint(Vec2(0.5, 0));
	this->addChild(tabBarBg, 1000);

	auto toolbar1 = MenuItemSprite::create(Sprite::createWithSpriteFrameName("xinxi1.png"), Sprite::createWithSpriteFrameName("xinxi2.png"), CC_CALLBACK_1(MainLayer::tapBarMenuPressed, this));
	auto toolbar2 = MenuItemSprite::create(Sprite::createWithSpriteFrameName("miyou1.png"), Sprite::createWithSpriteFrameName("miyou2.png"), CC_CALLBACK_1(MainLayer::tapBarMenuPressed, this));
	auto toolbar3 = MenuItemSprite::create(Sprite::createWithSpriteFrameName("faxian1.png"), Sprite::createWithSpriteFrameName("faxian2.png"), CC_CALLBACK_1(MainLayer::tapBarMenuPressed, this));
	auto toolbar4 = MenuItemSprite::create(Sprite::createWithSpriteFrameName("shezhi1.png"), Sprite::createWithSpriteFrameName("shezhi2.png"), CC_CALLBACK_1(MainLayer::tapBarMenuPressed, this));
	toolbar1->setTag(TOOLBARTAG + 0);
	toolbar2->setTag(TOOLBARTAG + 1);
	toolbar3->setTag(TOOLBARTAG + 2);
	toolbar4->setTag(TOOLBARTAG + 3);

	tabBarItems = Menu::create(toolbar1, toolbar2, toolbar3, toolbar4, NULL);
	tabBarItems->alignItemsHorizontallyWithPadding(80);
	tabBarItems->setPositionY(tabBarBg->getContentSize().height * 0.5f);

	tabBarBg->addChild(tabBarItems);

	m_topLayer = TopLayer::create();
	this->addChild(m_topLayer);
	//Director::getInstance()->setNotificationNode(m_topLayer);	//悬浮层 一直显示在屏幕上 不受场景切换的影响 这里是node被retain了一次
	//m_topLayer->onEnter();	//有内存泄露 没有释放悬浮层 不知道怎么释放
	
	//将csv文件写入到db文件中 只在第一次运行程序时执行
	//这里需要遍历所有的csv文件，将其中的内容写入到指定的表中 将文件名作为表名
	auto isfirst = UserDefault::getInstance()->getBoolForKey(IS_FIRST, false);	//第一次进来是false 默认是false
	if (!isfirst){
		std::vector<std::string> allcsv = { ALL_BOY, ALL_GIRL, BOY_DIALOG, GIRL_DIALOG };
		for (size_t i = 0; i < allcsv.size(); i++){
			CSVReader::getInstance()->CsvToSqlite3(allcsv.at(i).c_str(), split(allcsv.at(i), ".").at(0));
		}
		UserDefault::getInstance()->setBoolForKey(IS_FIRST, true);	//路径C:\Users\Administrator\AppData\Local\SecondGame\UserDefault.xml
	}
	//CSVReader::getInstance()->CsvToSqlite3("all_boy.csv", "all_boy");

	//现在的单例模式 只需要在一个地方创建一次就行  在其他地方都能用到单例的初始化对象 对象只有一个 在该地方释放就可以
	//怎么去弄呢 这是一个问题 每个人有4中属性
	//整理目前完成的任务
	/*
	1、对话界面实现
	2、数据库接口都写了
	3、列表界面
	4、多界面
	5、成功约到啦的图片的大小几文字的素材 ***
	6、聚会界面(当成功约到一个人物之后出现聚会提醒)***
	7、选择人物参加聚会的界面 需要做出一个人物列表出来 任务前面需要有勾选控件***
	*/


	return true;
}

void MainLayer::onEnter(){
	Layer::onEnter();
	layers->switchTo(0);
	static_cast<MenuItemSprite*>(tabBarItems->getChildByTag(TOOLBARTAG + 0))->selected();
}

void MainLayer::onExit(){
	Layer::onExit();
	
}

void MainLayer::tapBarMenuPressed(Ref* pSender){
	auto toolbar = static_cast<MenuItemSprite*>(pSender);
	toolbar->selected();
	layers->switchTo(toolbar->getTag() - TOOLBARTAG);

	m_topLayer->alterInfoWithIndex(toolbar->getTag() - TOOLBARTAG);

	for (int i = 0; i < tabBarItems->getChildren().size(); i++) {
		auto tmpbar = dynamic_cast<MenuItemSprite *>(tabBarItems->getChildren().at(i));
		if (tmpbar != NULL && tmpbar->getTag() != toolbar->getTag()) {
			tmpbar->unselected();
		}
	}
}