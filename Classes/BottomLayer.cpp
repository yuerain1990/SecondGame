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
	//�ڴ��ͷ�
	delete[] _firstLabel->getParent()->getUserData();
	delete[] _secondLabel->getParent()->getUserData();
}

bool BottomLayer::init(){
	if (!Layer::init()){
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();
	
	//�ײ�����
	_bottom_bg = Sprite::createWithSpriteFrameName("bottom_bg.png");
	_bottom_bg->setPosition(Vec2(winSize.width * 0.5, -_bottom_bg->getContentSize().height * 0.5 + 80));
	this->addChild(_bottom_bg);
	//�ײ���
	auto bottom = Sprite::createWithSpriteFrameName("bottom.png");
	bottom->setPosition(Vec2(winSize.width * 0.5, _bottom_bg->getContentSize().height - bottom->getContentSize().height * 0.5));
	_bottom_bg->addChild(bottom);
	//�����˵�
	auto help = Sprite::createWithSpriteFrameName("help.png");
	auto helpItem = MenuItemSprite::create(help, nullptr, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	helpItem->setTag(1);
	auto menuHelp = Menu::create(helpItem, nullptr);
	menuHelp->setPosition(Vec2(10 + help->getContentSize().width * 0.5, 14 + help->getContentSize().height * 0.5));
	bottom->addChild(menuHelp);
	//�����˵�
	auto pop = Sprite::createWithSpriteFrameName("pop1.png");
	_popItem = MenuItemSprite::create(pop, nullptr, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	_popItem->setTag(2); _popItem->setName(NO);
	auto menuPop = Menu::create(_popItem, nullptr);
	menuPop->setPosition(Vec2(bottom->getContentSize().width - 10 - pop->getContentSize().width * 0.5, 14 + pop->getContentSize().height * 0.5));
	bottom->addChild(menuPop);
	//�����
	auto frame = Sprite::createWithSpriteFrameName("frame.png");
	frame->setPosition(Vec2(winSize.width * 0.5, frame->getContentSize().height * 0.5 + 10));
	bottom->addChild(frame);
	//���ֻ��� ����������� ��Щ��һ��
	//��һ��
	auto firstWordNor = ui::Scale9Sprite::createWithSpriteFrameName("03.png");
	auto firstWordSel = ui::Scale9Sprite::createWithSpriteFrameName("04.png");
	firstWordNor->setContentSize(Size(600, 120));
	firstWordSel->setContentSize(Size(600, 120));
	//�ڶ���
	auto secondWordNor = ui::Scale9Sprite::createWithSpriteFrameName("03.png");
	auto secondWordSel = ui::Scale9Sprite::createWithSpriteFrameName("04.png");
	secondWordNor->setContentSize(Size(600, 120));
	secondWordSel->setContentSize(Size(600, 120));
	//����menu
	auto first = MenuItemSprite::create(firstWordNor, firstWordSel, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	auto second = MenuItemSprite::create(secondWordNor, secondWordSel, CC_CALLBACK_1(BottomLayer::menuCallback, this));
	first->setTag(3);
	second->setTag(4);
	auto menu = Menu::create(first, second, nullptr);
	menu->setPosition(Vec2(_bottom_bg->getContentSize().width * 0.5, _bottom_bg->getContentSize().height * 0.5 - 40));
	menu->alignItemsVerticallyWithPadding(20);
	_bottom_bg->addChild(menu);
	
	//label in menuitemsprite ��Ҫ�����ݿ���ȡ
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
	//��ȡDialogLayer
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
	//��ȡDialogLayer
	auto dialogLayer = static_cast<DialogLayer*>(this->getParent());
	//��ȡDialogLayer�ϵ�listview
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
			if (menuSprite->getName() == NO){			//�������ѡ���
				//_bottom_bg->runAction(moveBy);
				_bottom_bg->runAction(Sequence::create(moveBy, CallFunc::create([=](){
					this->listViewSizeChange(listView, 1);
				}), nullptr));
				menuSprite->setName(YES);
				pop->setSpriteFrame("pop2.png");

				//this->listViewSizeChange(listView, 1);
			}
			else if (menuSprite->getName() == YES){		//�������ѡ���
				_bottom_bg->runAction(moveBy->reverse());
				menuSprite->setName(NO);
				pop->setSpriteFrame("pop1.png");

				this->listViewSizeChange(listView, -1);
			}
		}
			break;
		case 3:
		{
			log("3333");	//����Ҫ��ʾ��listview�ϵ� ����Ҫ����Ի���
			this->sendMessage(menuSprite, dialogLayer, listView, moveBy);
		}
			break;
		case 4:
		{
			log("4444");	//����Ҫ��ʾ��listview�ϵ� ����Ҫ����Ի���
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
	//listView->scrollToPercentVertical(100.0f, 0.2f, false);	//���������Թ������ײ� �ڶ�������������false
}

void BottomLayer::resetLabelValueWithId(int id){
	this->destoryMemory();	//��������ǰ �������Ѿ����ڵ��ڴ�
	auto winSize = Director::getInstance()->getVisibleSize();
	//��ȡDialogLayer
	auto dialogLayer = static_cast<DialogLayer*>(this->getParent());
	//Me
	struct Dialog* info = dialogLayer->searchDialog(id, 1);
	log("%s %s %s %s %s", info->id, info->boy_id, info->boy_word, info->select_word, info->key_word_id);
	
	//��������Ҫȡ��select_word���� ����ֱ���� ��Ҫ�ȷָ����
	std::string str = info->select_word;
	std::vector<std::string> wordVec = split(str, ";");	//ȡ�������仰
	
	//�����и�������� �ҵ����ĳ���Ի����� ����ô����֪�� �Ҹõö��ٺøж���
	auto w1 = atoi(split(wordVec.at(0), "=").at(1).c_str());
	auto w2 = atoi(split(wordVec.at(1), "=").at(1).c_str());

	_firstLabel->setString(split(wordVec.at(0), "=").at(0));
	_secondLabel->setString(split(wordVec.at(1), "=").at(0));

	char* v1 = new char[100];
	char* v2 = new char[100];
	strcpy(v1, split(wordVec.at(0), "=").at(1).c_str());
	strcpy(v2, split(wordVec.at(1), "=").at(1).c_str());
	_firstLabel->getParent()->setUserData(v1);		//���ò˵���ť��ֵ
	_secondLabel->getParent()->setUserData(v2);		//���ò˵���ť��ֵ
	//delete[] v1;
	//delete[] v2;//�����ͷŲ��� userData��ûֵ ��Ҫ�������������ͷ�

	//��ֹ�ڴ�й¶
	//new delete��Ӧ newһ�� deleteһ��
	delete info;
}

void BottomLayer::destoryMemory(){
	//�ڴ��ͷ�
	delete[] _firstLabel->getParent()->getUserData();
	delete[] _secondLabel->getParent()->getUserData();
}

void BottomLayer::sendMessage(MenuItemSprite* menuSprite, DialogLayer* dialogLayer, ui::ListView* listView, MoveBy* moveBy){
	//***���ܳ���20 ������Ҫ����_no�����ݿ��� Ҳ��һ�������� �йؼ��� ��Ҫ��label�е�ֵ���ݵ�listview��
	auto str = static_cast<Label*>(menuSprite->getChildByName("lbl"))->getString();
	log("str=%s", str.c_str());
	dialogLayer->insertMeItemWithId(str);

	//����Ҫ�����ݴ���ȥ �жԻ����ݻ��кøж�ֵ �Ѻø�ֵ�ͽ�ɫһһ��Ӧ���� �½�һ���� �ֶ�Ϊ boy_id(�к����) score(�øж�) 
	auto v = (char*)menuSprite->getUserData();
	log("v=%s", v);	//��Ҫ�õ��øж�ֵ�ĵ����� ��ת����char* -> int atoi()

	struct DataBoy* gamedata = dialogLayer->searchGameData(1);
	_score = atof(gamedata->score);
	
	float tempScore = _score;	//��ʱ�ķ��� �����ж������ӻ��Ǽ���
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
		//������ʾ�� �ڹؼ���ش���� �ؼ���ش���ɺ� �ص�ԭ�ȵ���Ŀ���+1 
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
	//�����Ի���
	_popItem->setName(NO);
	auto pop = static_cast<Sprite*>(_popItem->getChildByTag(1));
	pop->setSpriteFrame("pop1.png");

	this->listViewSizeChange(listView, -1);

	if (_no == 16){
		//�жϵ���16��ʱ �ش���ɺ������� ��������80�ֵĻ� ����Լ��ʧ�ܵ���ʾ
		//�������ڻ����80�ֵĻ� ����Լ��ɹ�����ʾ Ȼ�󷵻ص��ɾͽ��� ������ʾ������Լ������
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
