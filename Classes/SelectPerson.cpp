//	Created by yuerain
#include "SelectPerson.h"
#include "Chinese.h"
#include "Row.h"

#define ROW_TAG 100
#define TOTAL 4

SelectPerson::SelectPerson()
{
}

SelectPerson::~SelectPerson()
{
}

bool SelectPerson::init(){
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 150))){
		return false;
	}
	//背景
	ui::ImageView* img = ui::ImageView::create("person.png");	//Size(274, 374)
	img->setPosition(Vec2(320, 568));
	this->addChild(img);

	//创建ui::ScrollView 垂直方向
	ui::ScrollView* scrollView = ui::ScrollView::create();	//ScrollView锚点默认是(0, 0)
	scrollView->setBounceEnabled(true);
	scrollView->setClippingEnabled(true);	//默认是true
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(250, 300));
	scrollView->setPosition(Vec2(10, 60));
	img->addChild(scrollView);
	//从数据库中取出的值 现在暂时写好 等后期再改***
	std::vector<std::string> nameVec = { "ZhangSan", "LiSi", "WangWu", "ZhaoLiu", "QianQi", "TangYi", "QiuXiang" };
	//行信息
	//这里加载的时候会比较慢 什么原因 怎么解决加载慢的问题呢 每一行都是要加载checkbox text 使用ui控件比较容易些
	for (size_t i = 0; i < nameVec.size(); i++)
	{
		auto row = Row::createWithName(nameVec.at(i));
		row->setPosition(Vec2(0, scrollView->getContentSize().height - i * 50));
		row->setTag(ROW_TAG + i);
		scrollView->addChild(row);
		rowVec.pushBack(row);
	}
	scrollView->setInnerContainerSize(Size(scrollView->getContentSize().width, 50 * nameVec.size()));
	
	//添加提示标签
	_tip = ui::Text::create();
	_tip->setString(StringUtils::format(Chinese::getInstance()->ChineseWord("tip").c_str(), 4));
	_tip->setFontSize(20);
	_tip->setPosition(Vec2(10, 20));
	_tip->setAnchorPoint(Vec2::ZERO);
	img->addChild(_tip);

	//确认按钮 点击之后 读取所勾选的4个人物 从rowVec中读取选中的4项
	ui::Button* button = ui::Button::create("btn_normal.png", "btn_select.png", "btn_disable.png");
	button->setPosition(Vec2(img->getContentSize().width * 0.5 + 70, 40));
	button->setTitleText("Certain");
	button->setTitleFontSize(25);
	button->addClickEventListener([=](Ref* sender){
		log("certain");
		int count = 0;
		for (int i = 0; i < rowVec.size(); i++){
			auto row = rowVec.at(i);
			if (row->getName() == "selected"){
				log("row tag = %d", row->getTag());								//得到tag
				log("name = %s", nameVec.at(row->getTag() - ROW_TAG).c_str());	//得到name
				++count;
			}
		}
		log("count = %d", count);
		scheduleOnce(schedule_selector(SelectPerson::discussResult), 180);		//延迟启动计时器180s 也就是3分钟
		//在等待的期间 我们怎么去处理这段时间应该做哪些事情呢 能不能做其他事情呢 还是只能静静的等待？？
		//应该列出一些对话内容 对话不能显示过快 第一条出现后隔5秒出现第二条，以此类推
	});
	img->addChild(button);
	_button = button;

	/*
	怎么及时更新tip的内容呢，我每点击一个checkbox应当记录一个值 该值要相应的+1
	*/

	schedule(schedule_selector(SelectPerson::updateData));
	//schedule(SEL_SCHEDULE(&SelectPerson::updateData));


	return true;
}

void SelectPerson::updateData(float dt){
	int count = 0;
	for (int i = 0; i < rowVec.size(); i++){
		auto row = rowVec.at(i);
		if (row->getName() == "selected"){		//出现一个就+1
			++count;
		}
	}
	if (count == 4){
		for (int i = 0; i < rowVec.size(); i++){
			auto row = rowVec.at(i);
			if (row->getName() == "unselected"){
				auto checkbox = static_cast<ui::CheckBox*>(row->getChildByName("checkbox"));
				checkbox->setEnabled(false);
				row->setEnabled(false);
				_button->setBright(true);
				_button->setEnabled(true);
				_button->setTouchEnabled(true);
			}
		}
	}
	else{
		for (int i = 0; i < rowVec.size(); i++){
			auto row = rowVec.at(i);
			if (row->getName() == "unselected"){
				auto checkbox = static_cast<ui::CheckBox*>(row->getChildByName("checkbox"));
				checkbox->setEnabled(true);
				row->setEnabled(true);
				_button->setBright(false);		//设置bright为false时 才能显示disable状态 但是此时点击了按钮不能显示select状态
				_button->setEnabled(false);
				_button->setTouchEnabled(false);
			}
		}
	}
	//如果count == 4的话 除了name = selected的checkbox可以点击外 其他的都不能点击
	this->setTipValue(count);
}

void SelectPerson::setTipValue(int count){
	_tip->setString(StringUtils::format(Chinese::getInstance()->ChineseWord("tip").c_str(), TOTAL - count));
}

void SelectPerson::discussResult(float dt){
	int sum = 0;
	for (int i = 0; i < rowVec.size(); i++){
		auto row = rowVec.at(i);
		if (row->getName() == "selected"){
			log("row tag = %d", row->getTag());								//得到tag
			//我要取出行所对应的人物，根据名字从数据库中查找 名字的话 需要保存起来
			sum += row->getTag() - ROW_TAG;
		}
	}
	log("sum=%d", sum);
	//两种情况 
	if (sum >= 15)
		log("success");	//1成功 将开通的人物编号写入到数据库中 回到主界面就可以更新界面了 
	else
		log("fail");	//2失败
}
//我现在可以取出每行的信息，怎么根据tag绑定人物信息呢？我现在只有一个tag而已
//根据name查找数据库中的元素select value1 from all_boy where name = 'ZhangSan'
//这次要取出value1，下次可以能要取出value2、value3、value4，每次都可能不同 那么就要动态的去传入这些数据
//不行啊 还是用更新函数了 虽然是调用耗时操作
//需要建lua项目才可以Lua绑定这么蛋疼的项目 真尼玛蛋疼 啊
//等待讨论结果 3分钟之后发送一个结果 经过激烈讨论。。。结果如下：
//
/*
整理思路：
点击确认按钮后，根据所选择的人物，取出需要的属性值，例如value1、value2、value3、value4其中一个
将这些数值进行相加，将结果和给定的一个值A进行比较，如果大于A的话，就聚餐成功，否则的话，聚餐失败，
成功的话，开通该人物，失败的话，暂时不开通该人物。
*/