#include "BottomLayer.h"
#include "DialogLayer.h"
#include "FunctionUtils.h"
#include "TipsLayer.h"
#include "DBUtil.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
#endif

#define YES "yes"
#define NO "no"
#define HEIGHT 300.0


BottomLayer::BottomLayer() : _no(1), _score(0.0)
{
	
}

BottomLayer::~BottomLayer()
{
	//内存释放
	delete[] _firstLabel->getParent()->getUserData();
	delete[] _secondLabel->getParent()->getUserData();
}

bool BottomLayer::init(){
	if (!Layer::init()){
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();
	
	//底部背景
	_bottom_bg = Sprite::createWithSpriteFrameName("bottom_bg.png");
	_bottom_bg->setPosition(Vec2(winSize.width * 0.5, -_bottom_bg->getContentSize().height * 0.5 + 80));
	this->addChild(_bottom_bg);
	//底部条
	auto bottom = Sprite::createWithSpriteFrameName("bottom.png");
	bottom->setPosition(Vec2(winSize.width * 0.5, _bottom_bg->getContentSize().height - bottom->getContentSize().height * 0.5));
	_bottom_bg->addChild(bottom);
	//帮助菜单
	auto help = Sprite::createWithSpriteFrameName("help.png");
	auto helpItem = MenuItemSprite::create(help, nullptr, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	helpItem->setTag(1);
	auto menuHelp = Menu::create(helpItem, nullptr);
	menuHelp->setPosition(Vec2(10 + help->getContentSize().width * 0.5, 14 + help->getContentSize().height * 0.5));
	bottom->addChild(menuHelp);
	//弹条菜单
	auto pop = Sprite::createWithSpriteFrameName("pop1.png");
	_popItem = MenuItemSprite::create(pop, nullptr, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	_popItem->setTag(2); _popItem->setName(NO);
	auto menuPop = Menu::create(_popItem, nullptr);
	menuPop->setPosition(Vec2(bottom->getContentSize().width - 10 - pop->getContentSize().width * 0.5, 14 + pop->getContentSize().height * 0.5));
	bottom->addChild(menuPop);
	//输入框
	auto frame = Sprite::createWithSpriteFrameName("frame.png");
	frame->setPosition(Vec2(winSize.width * 0.5, frame->getContentSize().height * 0.5 + 10));
	bottom->addChild(frame);
	//文字话语 大多数是两条 有些是一条
	//第一条
	auto firstWordNor = ui::Scale9Sprite::createWithSpriteFrameName("03.png");
	auto firstWordSel = ui::Scale9Sprite::createWithSpriteFrameName("04.png");
	firstWordNor->setContentSize(Size(600, 120));
	firstWordSel->setContentSize(Size(600, 120));
	//第二条
	auto secondWordNor = ui::Scale9Sprite::createWithSpriteFrameName("03.png");
	auto secondWordSel = ui::Scale9Sprite::createWithSpriteFrameName("04.png");
	secondWordNor->setContentSize(Size(600, 120));
	secondWordSel->setContentSize(Size(600, 120));
	//话语menu
	auto first = MenuItemSprite::create(firstWordNor, firstWordSel, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	auto second = MenuItemSprite::create(secondWordNor, secondWordSel, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	first->setTag(3);
	second->setTag(4);
	auto menu = Menu::create(first, second, nullptr);
	menu->setPosition(Vec2(_bottom_bg->getContentSize().width * 0.5, _bottom_bg->getContentSize().height * 0.5 - 40));
	menu->alignItemsVerticallyWithPadding(20);
	_bottom_bg->addChild(menu);
	
	//label in menuitemsprite 需要从数据库中取
	//firstlabel
	_firstLabel = Label::createWithSystemFont("You and me,me and you,don't worry,be be be behappy!", "Arial", 25);
	_firstLabel->setPosition(Vec2(first->getContentSize().width * 0.5 + 10, first->getContentSize().height * 0.5));
	_firstLabel->setColor(Color3B::BLACK);
	_firstLabel->setDimensions(600 - 20, 0);
	_firstLabel->setName("lbl");
	first->addChild(_firstLabel);
	//secondlabel
	_secondLabel = Label::createWithSystemFont("You and me,me and you,don't worry!", "Arial", 25);
	_secondLabel->setPosition(Vec2(second->getContentSize().width * 0.5 + 10, second->getContentSize().height * 0.5));
	_secondLabel->setColor(Color3B::BLACK);
	_secondLabel->setDimensions(600 - 20, 0);
	_secondLabel->setName("lbl");
	second->addChild(_secondLabel);
	

	return true;
}

void BottomLayer::onEnter(){
	Layer::onEnter();
	//获取DialogLayer
	auto dialogLayer = static_cast<DialogLayer*>(this->getParent());
	_no = dialogLayer->getNo();
	this->resetLabelValueWithId(_no);
}

void BottomLayer::onExit(){
	Layer::onExit();
}

void BottomLayer::menuCallback(Ref* sender){
	auto menuSprite = static_cast<MenuItemSprite*>(sender);
	auto tag = menuSprite->getTag();
	//获取DialogLayer
	auto dialogLayer = static_cast<DialogLayer*>(this->getParent());
	//获取DialogLayer上的listview
	auto listView = dialogLayer->getListView();
	auto moveBy = MoveBy::create(0.2f, Vec2(0, HEIGHT));
	switch (tag)
	{
		case 1:
		{
			log("1111");
		}
			break;
		case 2:
		{
			log("2222");
			
			auto pop = static_cast<Sprite*>(menuSprite->getChildByTag(1));
			if (menuSprite->getName() == NO){			//弹出语句选择框
				//_bottom_bg->runAction(moveBy);
				_bottom_bg->runAction(Sequence::create(moveBy, CallFunc::create([=](){
					this->listViewSizeChange(listView, 1);
				}), nullptr));
				menuSprite->setName(YES);
				pop->setSpriteFrame("pop2.png");

				//this->listViewSizeChange(listView, 1);
			}
			else if (menuSprite->getName() == YES){		//收起语句选择框
				_bottom_bg->runAction(moveBy->reverse());
				menuSprite->setName(NO);
				pop->setSpriteFrame("pop1.png");

				this->listViewSizeChange(listView, -1);
			}
		}
			break;
		case 3:
		{
			log("3333");	//将来要显示到listview上的 并且要收起对话框
			this->sendMessage(menuSprite, dialogLayer, listView, moveBy);
		}
			break;
		case 4:
		{
			log("4444");	//将来要显示到listview上的 并且要收起对话框
			this->sendMessage(menuSprite, dialogLayer, listView, moveBy);
		}
			break;
		default:
			break;
	}
}

void BottomLayer::listViewSizeChange(ui::ListView* listView, int factor){
	listView->setPosition(listView->getPosition() + Vec2(0, HEIGHT * factor));
	log("x=%f y=%f", listView->getPosition().x, listView->getPosition().y);
	listView->setContentSize(Size(listView->getContentSize().width, listView->getContentSize().height - HEIGHT * factor));
	listView->scrollToBottom(0.2f, false);
	//listView->scrollToPercentVertical(100.0f, 0.2f, false);	//两个都可以滚动到底部 第二个参数必须是false
}

void BottomLayer::resetLabelValueWithId(int id){
	this->destoryMemory();	//重新设置前 先销毁已经存在的内存
	auto winSize = Director::getInstance()->getVisibleSize();
	//获取DialogLayer
	auto dialogLayer = static_cast<DialogLayer*>(this->getParent());
	//Me
	struct Dialog* info = dialogLayer->searchDialog(id, 1);
	log("%s %s %s %s %s", info->id, info->boy_id, info->boy_word, info->select_word, info->key_word_id);
	
	//我现在需要取出select_word出来 不能直接用 需要先分割出来
	std::string str = info->select_word;
	std::vector<std::string> wordVec = split(str, ";");	//取出这两句话
	
	//现在有个问题就是 我点击了某个对话内容 我怎么才能知道 我该得多少好感度呢
	auto w1 = atoi(split(wordVec.at(0), "=").at(1).c_str());
	auto w2 = atoi(split(wordVec.at(1), "=").at(1).c_str());

	_firstLabel->setString(split(wordVec.at(0), "=").at(0));
	_secondLabel->setString(split(wordVec.at(1), "=").at(0));

	char* v1 = new char[100];
	char* v2 = new char[100];
	strcpy(v1, split(wordVec.at(0), "=").at(1).c_str());
	strcpy(v2, split(wordVec.at(1), "=").at(1).c_str());
	_firstLabel->getParent()->setUserData(v1);		//设置菜单按钮的值
	_secondLabel->getParent()->setUserData(v2);		//设置菜单按钮的值
	//delete[] v1;
	//delete[] v2;//这里释放不行 userData会没值 需要在析构函数中释放

	//防止内存泄露
	//new delete对应 new一个 delete一个
	delete info;
}

void BottomLayer::destoryMemory(){
	//内存释放
	delete[] _firstLabel->getParent()->getUserData();
	delete[] _secondLabel->getParent()->getUserData();
}

void BottomLayer::sendMessage(MenuItemSprite* menuSprite, DialogLayer* dialogLayer, ui::ListView* listView, MoveBy* moveBy){
	//***不能超过20 并且需要保存_no到数据库中 也不一定是自增 有关键题 需要将label中的值传递到listview上
	auto str = static_cast<Label*>(menuSprite->getChildByName("lbl"))->getString();
	log("str=%s", str.c_str());
	dialogLayer->insertMeItemWithId(str);

	//我需要把内容传过去 有对话内容还有好感度值 把好感值和角色一一对应起来 新建一个表 字段为 boy_id(男孩编号) score(好感度) 
	auto v = (char*)menuSprite->getUserData();
	log("v=%s", v);	//需要用到好感度值的叠加中 先转换成char* -> int atoi()

	struct DataBoy* gamedata = dialogLayer->searchGameData(1);
	_score = atof(gamedata->score);
	
	float tempScore = _score;	//临时的分数 用于判断是增加还是减少
	Color3B col(Color3B::WHITE);
	_score += atof(v);
	if (_score >= tempScore){
		col = Color3B::GREEN;
	}
	else{
		col = Color3B::RED;
		
		int temp_no = 0;
		switch (_no)
		{
			case 4:
			{
				temp_no = 17;
			}
				break;
			case 8:
			{
				temp_no = 18;
			}
				break;
			case 12:
			{
				temp_no = 19;
			}
				break;
			case 15:
			{
				temp_no = 20;
			}
				break;
			default:
				break;
		}
		//弹出提示框 在关键题回答错误 关键题回答完成后 回到原先的题目编号+1 
		if (_no == 4 || _no == 8 || _no == 12 || _no == 15){
			this->getParent()->addChild(TipsLayer::create());
			dialogLayer->alterHeartScore(_score, v, col);
			this->resetLabelValueWithId(temp_no);
			dialogLayer->insertYouItemWithId(temp_no, (int)_score, str);
			return;
		}
	}
	dialogLayer->alterHeartScore(_score, v, col);

	_bottom_bg->runAction(moveBy->reverse());
	//弹出对话框
	_popItem->setName(NO);
	auto pop = static_cast<Sprite*>(_popItem->getChildByTag(1));
	pop->setSpriteFrame("pop1.png");

	this->listViewSizeChange(listView, -1);

	if (_no == 16){
		//判断到第16题时 回答完成后计算分数 分数不够80分的话 弹出约会失败的提示
		//分数大于或等于80分的话 弹出约会成功的提示 然后返回到成就界面 这里显示你所有约到的人
		if (_score >= 80)
			log("success");
		else
			log("failed");
		return;
	}
	this->resetLabelValueWithId(++_no);
	dialogLayer->insertYouItemWithId(_no, (int)_score, str);

	delete gamedata;
}
