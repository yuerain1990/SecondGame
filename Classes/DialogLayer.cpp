#include "DialogLayer.h"
#include "TopLayer.h"
#include "BottomLayer.h"
#include "FunctionUtils.h"
#include "DBUtil.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
#endif

DialogLayer::DialogLayer() : _no(1)
{
	
}

DialogLayer::~DialogLayer()
{
	
}

Scene* DialogLayer::createSceneWithIndex(int index){
	auto scene = Scene::create();
	auto layer = DialogLayer::createWithIndex(index);
	scene->addChild(layer);

	return scene;
}

DialogLayer* DialogLayer::createWithIndex(int index){
	auto ret = new DialogLayer();
	if (ret && ret->initWithIndex(index)){
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool DialogLayer::initWithIndex(int index){
	if (!LayerColor::initWithColor(Color4B::MAGENTA)){
		return false;
	}
	_index = index;
	//现在我能传进需要对话对对象的index 下一步是要从数据库中取数据 根据index查询数据
	winSize = Director::getInstance()->getVisibleSize();
	auto wSize = Director::getInstance()->getWinSize();

	//ceshi
	this->createListView();
	//you me 这里默认打开时显示第一条对话 还需要实现不是第一次和他对话的 current_word_id有值 不为0 的情况
	//this->insertYouItemWithId(1, 0, "");
	this->setListViewContent();

	_topLayer = TopLayer::create();
	_bottomLayer = BottomLayer::create();
	this->addChild(_topLayer);		//顶部
	this->addChild(_bottomLayer);	//底部

	//现在我只有一条数据 boy id = 1 一进来 就添加boy的第一条数据 是第一次进来的话

	//添加back按钮 只在对话界面才有的
	auto label = Label::createWithTTF("Back", "fonts/arial.ttf", 35);
	//label->setColor(Color3B::BLACK);
	auto back = MenuItemLabel::create(label, CC_CALLBACK_1(DialogLayer::menuCallback, this));
	back->setTag(1);
	auto menu = Menu::create(back, nullptr);
	menu->setPosition(Vec2(50, winSize.height - 100));
	this->addChild(menu);

	//添加好感度图片
	//底图
	auto heart_01 = Sprite::create("heart_01.png");
	heart_01->setPosition(Vec2(winSize.width - 60, winSize.height - 110));
	this->addChild(heart_01);
	//变化图
	auto heart_02 = ProgressTimer::create(Sprite::create("heart_02.png"));
	heart_02->setType(ProgressTimer::Type::BAR);
	heart_02->setMidpoint(Vec2(0, 0));	//从下往上升
	heart_02->setBarChangeRate(Vec2(0, 1));
	heart_02->setPosition(heart_01->getContentSize() * 0.5);
	heart_01->addChild(heart_02);
	_heart = heart_02;

	_heart->setPercentage(_score);	//设置默认进度 此时满心的值为80，一般是100 换算公式为_score * 100 / 80

	//auto to1 = Sequence::createWithTwoActions(ProgressTo::create(2, 100), ProgressTo::create(0, 0));
	//heart_02->runAction(RepeatForever::create(to1));


	return true;
}
//只适用在TTF文件中，因为我还需要支持中文
std::string DialogLayer::myWrap(std::string str, int length)
{
	unsigned int beginPos = 0;  //字符串的初始位置
	std::string resultStr;		//返回的字符串

	std::vector<std::string> str_vec;  //创建一个字符串类型的顺序容器
	do
	{
		str_vec.push_back(str.substr(beginPos, length)); //substr函数的作用类似剪刀，将str中从beginPos到length之间的字符串剪下来，单独放入容器中 
		if (beginPos + length >str.size())
		{
			break;  //当要裁剪的长度超出str的长度，则退出循环
		}
		else
		{
			beginPos += length;
		}

	} while (true);

	for (unsigned int i = 0; i<str_vec.size(); ++i)
	{
		resultStr.append(str_vec.at(i)).append("\n"); //从容器逐一取出之前裁剪好的一段段字符串，分别在字符串后面加上换行符。append()类似胶水，将\n粘到字符串后面 
	}

	resultStr.pop_back();  //这一句是将最后一个多余的\n给删掉

	return resultStr;
}

Scale9Sprite* DialogLayer::youWordWithId(int id, int score, std::string meword){
	_no = id;
	//setNo(id);
	auto winSize = Director::getInstance()->getVisibleSize();
	//You
	std::string sql = StringUtils::format("select * from boy_dialog where id = %d and boy_id = %d", id, 1);//男孩1的第N句话 1是变化的 需要传进来的值
	//struct Info* info = (struct Info*)malloc(sizeof(struct Info)); //malloc 或 new 都可以
	//free(info);
	struct Dialog* info = this->searchDialog(id, 1);
	log("%s %s %s %s %s", info->id, info->boy_id, info->boy_word, info->select_word, info->key_word_id);

	std::string str = info->boy_word;
	//对话框 九宫格
	auto label = Label::createWithSystemFont(/*this->myWrap(str, 30)*/str, "Arial", 25);
	label->setColor(Color3B::BLACK);
	label->setDimensions(400, 0);
	//label->setPosition(Vec2(320, 568));
	//this->addChild(label);

	auto sp = ui::Scale9Sprite::createWithSpriteFrameName("01.png");
	Size spSize = Size(label->getContentSize().width + 40, label->getContentSize().height + 30);
	sp->setContentSize(spSize);
	sp->addChild(label);
	label->setPosition(sp->getContentSize() / 2);
	sp->setAnchorPoint(Vec2(0, 0));
	//sp->setPosition(Vec2(40, 768));
	//this->addChild(sp);

	//三角
	auto sj = Sprite::createWithSpriteFrameName("1.png");
	sj->setFlippedX(false);			//x轴翻转
	sj->setAnchorPoint(Vec2(1, 0));
	sj->setPosition(Vec2(2, sp->getContentSize().height - 30));
	sp->addChild(sj);

	//时间
	auto sec = getSecondTime();		//获取秒数
	auto cur = getCurrentTime();	//将秒数转化为时间格式
	auto timeLabel = Label::createWithTTF(StringUtils::format("%s", cur.c_str()), "fonts/arial.ttf", 20);
	timeLabel->setColor(Color3B::BLACK);
	timeLabel->setAnchorPoint(Vec2(0, 0));
	timeLabel->setPosition(Vec2(sp->getContentSize().width + 5, 0));
	sp->addChild(timeLabel);


	struct DataBoy* gamedata = this->searchGameData(1);//改
	std::string you_word = gamedata->you_word;
	std::string me_word = gamedata->me_word;
	you_word.append(info->boy_word).append("=").append(cur).append("|");
	if (meword != ""){
		me_word.append(meword).append("=").append(cur).append("|");
	}
	sql = StringUtils::format("update game_data set score = %d, you_word = '%s', me_word = '%s' where boy_id = 1", score, you_word.c_str(), me_word.c_str());
	DBUtil::getInstance()->updateData(sql);
	
	//防止内存泄露
	//new delete对应 new一个 delete一个
	delete info;
	delete gamedata;

	return sp;
}

ui::Scale9Sprite* DialogLayer::meWordWithId(std::string str){
	auto winSize = Director::getInstance()->getVisibleSize();
	//对话框 九宫格
	auto label = Label::createWithSystemFont(str, "Arial", 25);
	label->setColor(Color3B::BLACK);
	label->setDimensions(400, 0);
	//label->setPosition(Vec2(320, 568));
	//this->addChild(label);

	auto sp = ui::Scale9Sprite::createWithSpriteFrameName("02.png");
	Size spSize = Size(label->getContentSize().width + 40, label->getContentSize().height + 30);
	sp->setContentSize(spSize);
	sp->addChild(label);
	label->setPosition(sp->getContentSize() / 2);
	sp->setAnchorPoint(Vec2(1, 0));
	//sp->setPosition(Vec2(winSize.width - 40, 568));
	//this->addChild(sp);

	//三角
	auto sj = Sprite::createWithSpriteFrameName("2.png");
	sj->setFlippedX(true);			//x轴翻转
	sj->setAnchorPoint(Vec2::ZERO);
	sj->setPosition(Vec2(sp->getContentSize().width - 2, sp->getContentSize().height - 30));
	sp->addChild(sj);

	//时间
	auto sec = getSecondTime();		//获取秒数
	auto cur = getCurrentTime();	//将秒数转化为时间格式

	auto timeLabel = Label::createWithTTF(StringUtils::format("%s", cur.c_str()), "fonts/arial.ttf", 20);
	timeLabel->setColor(Color3B::BLACK);
	timeLabel->setAnchorPoint(Vec2(1, 0));
	timeLabel->setPosition(Vec2(-5, 0));
	sp->addChild(timeLabel);

	return sp;
}

void DialogLayer::createListView(){
	auto listView = ui::ListView::create();							//创建ListView
	listView->setDirection(ui::ScrollView::Direction::VERTICAL);	//设置滚动方向
	listView->setBounceEnabled(true);								//设置可回弹
	listView->setBackGroundImage("listback.png");					//设置ListView的背景图片
	listView->setBackGroundImageScale9Enabled(true);				//设置背景图片支持9妹
	listView->setContentSize(Size(winSize.width, winSize.height - 130 - 80));
	listView->setPosition(Vec2(0, 80));
	listView->setAnchorPoint(Vec2::ZERO);
	listView->setClippingEnabled(true);								//是否裁剪
	listView->scrollToBottom(0.2f, false);
	this->addChild(listView);

	listView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);	//设置对齐方式
	listView->setItemsMargin(20);									//设置item之间的间隙
	listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(DialogLayer::selectedItemEvent, this));
	_listView = listView;
}

//事件监听还有问题 等以后再弄
void DialogLayer::selectedItemEvent(Ref* pSender, ui::ListView::EventType type){
	switch (type)
	{
		case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
			break;
		case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
		{
			ui::ListView* listView = static_cast<ui::ListView*>(pSender);
			CC_UNUSED_PARAM(listView);
			log("%d", listView->getCurSelectedIndex());

			break;
		}
		default:
			break;
	}
}

void DialogLayer::insertYouItemWithId(int id, int score, std::string str){
	if (!_listView || id > 20){
		return;
	}
	Vector<ui::Widget*>& items = _listView->getItems();
	size_t items_count = items.size();
	auto sp = youWordWithId(id, score, str);
	ui::Layout* item = ui::Layout::create();
	item->setContentSize(sp->getContentSize());
	sp->setPosition(Vec2(40, 0));
	item->addChild(sp);

	////添加头像 这里有问题 能显示头像 但是不能显示对话框??需要修改代码
	//auto headSprite = Sprite::createWithSpriteFrameName(StringUtils::format("girl%03d.png", 0));
	//ClippingNode* headNode = ccDrawRoundRect(headSprite, Vec2(-50, -50), Vec2(50, 50), 15, 1000);
	//headNode->setPosition(Vec2(20, 20));
	//item->addChild(headNode);

	_listView->insertCustomItem(item, items_count);
}

void DialogLayer::insertMeItemWithId(std::string  str){
	if (!_listView){
		return;
	}
	Vector<ui::Widget*>& items = _listView->getItems();
	size_t items_count = items.size();
	auto sp = meWordWithId(str);
	ui::Layout* item = ui::Layout::create();
	item->setContentSize(sp->getContentSize());
	sp->setPosition(Vec2(winSize.width - 40, 0));
	item->addChild(sp);
	_listView->insertCustomItem(item, items_count);
}

void DialogLayer::onEnter(){
	LayerColor::onEnter();
	_heart->setPercentage(_score);	//此时满心的值为80，一般是100 换算公式为_score * 100 / 80
}

void DialogLayer::onExit(){
	LayerColor::onExit();
	
}

ui::ListView* DialogLayer::getListView(){
	return _listView;
}

void DialogLayer::menuCallback(Ref* pSender){
	auto tmpItem = static_cast<MenuItemSprite*>(pSender);
	switch (tmpItem->getTag())
	{
		case 1:
		{
			log("tag 1 clicked");
			//点击返回按钮时 更新当前对象的的话语编号 要不要每添加you_word就将current_word_id写入到数据库中呢???
			std::string sql = StringUtils::format("update game_data set current_word_id = %d where boy_id = 1", _no);
			DBUtil::getInstance()->updateData(sql);
			Director::getInstance()->popScene();	
		}
			break;
	}
}

void DialogLayer::alterHeartScore(float score, const char* v, Color3B& color){
	_heart->setPercentage(score);
	auto vlbl = Label::createWithSystemFont(v, "Arial", 35);
	vlbl->setPosition(Vec2(_heart->getContentSize().width * 0.5, vlbl->getContentSize().height * 0.5));
	vlbl->setColor(color);
	_heart->addChild(vlbl);
	auto moveBy = MoveBy::create(0.5f, Vec2(0, 50));
	vlbl->runAction(Sequence::create(moveBy, CallFunc::create([=](){
		vlbl->removeFromParent();
	}), nullptr));
}

struct DataBoy* DialogLayer::searchGameData(int index){
	std::string sql1 = StringUtils::format("select count(*) from game_data where boy_id = %d", index);
	std::string sql2 = StringUtils::format("select * from game_data where boy_id = %d", index);
	std::string sql3 = StringUtils::format("insert into game_data (boy_id) values (%d)", index);
	std::vector<std::map<std::string, std::string>> vec = DBUtil::getInstance()->searchData(sql1, sql2, sql3);//改
	struct DataBoy* data = new DataBoy;
	for (size_t i = 0; i < vec.size(); i++)
	{
		std::map<std::string, std::string>::iterator iter;
		for (iter = vec[i].begin(); iter != vec[i].end(); ++iter){
			log("name=%s,value=%s", iter->first.c_str(), iter->second.c_str());
			if (iter->first == "boy_id"){
				strcpy(data->boy_id, iter->second.c_str());
			}
			if (iter->first == "score"){
				if (iter->second == ""){
					strcpy(data->score, "0");
				}
				else{
					strcpy(data->score, iter->second.c_str());
				}
			}
			if (iter->first == "current_word_id"){
				if (iter->second == ""){
					strcpy(data->current_word_id, "0");
				}
				else{
					strcpy(data->current_word_id, iter->second.c_str());
				}
			}
			if (iter->first == "you_word"){
				strcpy(data->you_word, iter->second.c_str());
			}
			if (iter->first == "me_word"){
				strcpy(data->me_word, iter->second.c_str());
			}
		}
	}
	
	return data;
}

struct Dialog* DialogLayer::searchDialog(int pid, int pboyid){
	std::string sql = StringUtils::format("select * from boy_dialog where id = %d and boy_id = %d", pid, pboyid);
	std::vector<std::map<std::string, std::string>> vec = DBUtil::getInstance()->searchData(sql);//改
	struct Dialog* data = new Dialog;
	for (size_t i = 0; i < vec.size(); i++)
	{
		std::map<std::string, std::string>::iterator iter;
		for (iter = vec[i].begin(); iter != vec[i].end(); ++iter){
			log("name=%s,value=%s", iter->first.c_str(), iter->second.c_str());
			if (iter->first == "id"){
				strcpy(data->id, iter->second.c_str());
			}
			if (iter->first == "boy_id"){
				strcpy(data->boy_id, iter->second.c_str());
			}
			if (iter->first == "boy_word"){
				strcpy(data->boy_word, iter->second.c_str());
			}
			if (iter->first == "select_word"){
				strcpy(data->select_word, iter->second.c_str());
			}
			if (iter->first == "key_word_id"){
				strcpy(data->key_word_id, iter->second.c_str());
			}
		}
	}
	
	return data;
}

void DialogLayer::setListViewContent(){
	struct DataBoy* gamedata = this->searchGameData(1);
	std::string current_word_id = gamedata->current_word_id;
	if (current_word_id == "0"){
		this->insertYouItemWithId(1, 0, "");
		delete gamedata;
		return;
	}
	_no = atoi(current_word_id.c_str());
	std::string you_word = gamedata->you_word;
	std::string me_word = gamedata->me_word;
	std::string score = gamedata->score;
	_score = atof(score.c_str());
	//取出所有的话you
	std::vector<std::string> youword_vec = split(you_word, "|");
	//取出所有的话me
	std::vector<std::string> meword_vec = split(me_word, "|");
	//接下来需要遍历话语向量
	for (int i = 0; i < youword_vec.size(); i++){
		auto y = split(youword_vec.at(i), "=");			//你的话
		this->youWordWithContent(y.at(0), y.at(1));		//内容 时间
		if (i < meword_vec.size()){						//youword_vec.size() - 1
			auto m = split(meword_vec.at(i), "=");		//我的话
			this->meWordWithContent(m.at(0), m.at(1));	//内容 时间d
		}
	}

	delete gamedata;
}
/**
 *@pStr  内容
 *@pTime 时间
 */
void DialogLayer::youWordWithContent(std::string pStr, std::string pTime){
	auto winSize = Director::getInstance()->getVisibleSize();
	//对话框 九宫格
	auto label = Label::createWithSystemFont(pStr, "Arial", 25);
	label->setColor(Color3B::BLACK);
	label->setDimensions(400, 0);
	//背景
	auto sp = ui::Scale9Sprite::createWithSpriteFrameName("01.png");
	Size spSize = Size(label->getContentSize().width + 40, label->getContentSize().height + 30);
	sp->setContentSize(spSize);
	sp->addChild(label);
	label->setPosition(sp->getContentSize() / 2);
	sp->setAnchorPoint(Vec2(0, 0));
	//三角
	auto sj = Sprite::createWithSpriteFrameName("1.png");
	sj->setFlippedX(false);			//x轴翻转
	sj->setAnchorPoint(Vec2(1, 0));
	sj->setPosition(Vec2(2, sp->getContentSize().height - 30));
	sp->addChild(sj);
	//时间
	auto timeLabel = Label::createWithTTF(pTime, "fonts/arial.ttf", 20);
	timeLabel->setColor(Color3B::BLACK);
	timeLabel->setAnchorPoint(Vec2(0, 0));
	timeLabel->setPosition(Vec2(sp->getContentSize().width + 5, 0));
	sp->addChild(timeLabel);
	//添加到ListView上
	if (!_listView){
		return;
	}
	Vector<ui::Widget*>& items = _listView->getItems();
	size_t items_count = items.size();
	ui::Layout* item = ui::Layout::create();
	item->setContentSize(sp->getContentSize());
	sp->setPosition(Vec2(40, 0));
	item->addChild(sp);
	_listView->insertCustomItem(item, items_count);
}
/**
 *@pStr  内容
 *@pTime 时间
 */
void DialogLayer::meWordWithContent(std::string pStr, std::string pTime){
	auto winSize = Director::getInstance()->getVisibleSize();
	//对话框 九宫格
	auto label = Label::createWithSystemFont(pStr, "Arial", 25);
	label->setColor(Color3B::BLACK);
	label->setDimensions(400, 0);
	//背景
	auto sp = ui::Scale9Sprite::createWithSpriteFrameName("02.png");
	Size spSize = Size(label->getContentSize().width + 40, label->getContentSize().height + 30);
	sp->setContentSize(spSize);
	sp->addChild(label);
	label->setPosition(sp->getContentSize() / 2);
	sp->setAnchorPoint(Vec2(1, 0));
	//三角
	auto sj = Sprite::createWithSpriteFrameName("2.png");
	sj->setFlippedX(true);			//x轴翻转
	sj->setAnchorPoint(Vec2::ZERO);
	sj->setPosition(Vec2(sp->getContentSize().width - 2, sp->getContentSize().height - 30));
	sp->addChild(sj);
	//时间
	auto timeLabel = Label::createWithTTF(pTime, "fonts/arial.ttf", 20);
	timeLabel->setColor(Color3B::BLACK);
	timeLabel->setAnchorPoint(Vec2(1, 0));
	timeLabel->setPosition(Vec2(-5, 0));
	sp->addChild(timeLabel);
	//添加到ListView上
	if (!_listView){
		return;
	}
	Vector<ui::Widget*>& items = _listView->getItems();
	size_t items_count = items.size();
	ui::Layout* item = ui::Layout::create();
	item->setContentSize(sp->getContentSize());
	sp->setPosition(Vec2(winSize.width - 40, 0));
	item->addChild(sp);
	_listView->insertCustomItem(item, items_count);
}