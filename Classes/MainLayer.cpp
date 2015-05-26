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
	//����������
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
	//Director::getInstance()->setNotificationNode(m_topLayer);	//������ һֱ��ʾ����Ļ�� ���ܳ����л���Ӱ�� ������node��retain��һ��
	//m_topLayer->onEnter();	//���ڴ�й¶ û���ͷ������� ��֪����ô�ͷ�
	
	//��csv�ļ�д�뵽db�ļ��� ֻ�ڵ�һ�����г���ʱִ��
	//������Ҫ�������е�csv�ļ��������е�����д�뵽ָ���ı��� ���ļ�����Ϊ����
	auto isfirst = UserDefault::getInstance()->getBoolForKey(IS_FIRST, false);	//��һ�ν�����false Ĭ����false
	if (!isfirst){
		std::vector<std::string> allcsv = { ALL_BOY, ALL_GIRL, BOY_DIALOG, GIRL_DIALOG };
		for (size_t i = 0; i < allcsv.size(); i++){
			CSVReader::getInstance()->CsvToSqlite3(allcsv.at(i).c_str(), split(allcsv.at(i), ".").at(0));
		}
		UserDefault::getInstance()->setBoolForKey(IS_FIRST, true);	//·��C:\Users\Administrator\AppData\Local\SecondGame\UserDefault.xml
	}
	//CSVReader::getInstance()->CsvToSqlite3("all_boy.csv", "all_boy");

	//���ڵĵ���ģʽ ֻ��Ҫ��һ���ط�����һ�ξ���  �������ط������õ������ĳ�ʼ������ ����ֻ��һ�� �ڸõط��ͷžͿ���
	//��ôȥŪ�� ����һ������ ÿ������4������
	//����Ŀǰ��ɵ�����
	/*
	1���Ի�����ʵ��
	2�����ݿ�ӿڶ�д��
	3���б����
	4�������
	5���ɹ�Լ������ͼƬ�Ĵ�С�����ֵ��ز� ***
	6���ۻ����(���ɹ�Լ��һ������֮����־ۻ�����)***
	7��ѡ������μӾۻ�Ľ��� ��Ҫ����һ�������б���� ����ǰ����Ҫ�й�ѡ�ؼ�***
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