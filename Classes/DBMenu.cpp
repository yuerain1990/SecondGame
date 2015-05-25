#include "DBMenu.h"

bool DBMenu::init()
{
	return initWithArray(Vector<MenuItem*>());
}

bool DBMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
	if (Layer::init())
	{
		_enabled = true;
		// menu in the center of the screen
		Size s = Director::getInstance()->getWinSize();

		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(Vec2(0.5f, 0.5f));
		this->setContentSize(s);

		setPosition(Vec2(s.width / 2, s.height / 2));

		int z = 0;

		for (auto& item : arrayOfItems)
		{
			this->addChild(item, z);
			z++;
		}

		_selectedItem = nullptr;
		_state = Menu::State::WAITING;

		// enable cascade color and opacity on menus
		setCascadeColorEnabled(true);
		setCascadeOpacityEnabled(true);


		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(false);

		touchListener->onTouchBegan = CC_CALLBACK_2(DBMenu::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(DBMenu::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(DBMenu::onTouchEnded, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(DBMenu::onTouchCancelled, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		return true;
	}
	return false;
}

DBMenu* DBMenu::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
	auto ret = new DBMenu();
	if (ret && ret->initWithArray(arrayOfItems))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

DBMenu* DBMenu::createWithItem(MenuItem* item)
{
	return DBMenu::create(item, nullptr);
}

DBMenu* DBMenu::createWithItems(MenuItem *item, va_list args)
{
	Vector<MenuItem*> items;
	if (item)
	{
		items.pushBack(item);
		MenuItem *i = va_arg(args, MenuItem*);
		while (i)
		{
			items.pushBack(i);
			i = va_arg(args, MenuItem*);
		}
	}

	return DBMenu::createWithArray(items);
}

DBMenu* DBMenu::create(MenuItem* item, ...) //CC_REQUIRES_NULL_TERMINATION
{
	va_list args;
	va_start(args, item);

	DBMenu *ret = DBMenu::createWithItems(item, args);

	va_end(args);

	return ret;
}

bool DBMenu::onTouchBegan(Touch* touch, Event* event)
{
	if (_state != Menu::State::WAITING || !_visible || !_enabled)
	{
		return false;
	}

	for (Node *c = this->_parent; c != nullptr; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}
	//menuitem在可视区域外被触发的问题 解决方法
	if (m_bCheckScissor){
		GLView* eglView = Director::getInstance()->getOpenGLView();
		const Rect& scissorBox = eglView->getScissorRect();
		if (!scissorBox.containsPoint(touch->getLocation())){
			return false;
		}
	}

	_selectedItem = this->getItemForTouch(touch);
	if (_selectedItem)
	{
		_state = Menu::State::TRACKING_TOUCH;
		_selectedItem->selected();

		//实现滚动ScrollView，不触发Menu的点击事件1.
		auto si = isSwallowsTouches();
		if (!isSwallowsTouches()){
			m_touchBeginWorldPos = convertToWorldSpace(getPosition());
		}

		return true;
	}

	return false;
}

void DBMenu::onTouchEnded(Touch* touch, Event* event)
{
	CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
	this->retain();
	if (_selectedItem)
	{
		_selectedItem->unselected();
		//实现滚动ScrollView，不触发Menu的点击事件2.
		do
		{
			if (!isSwallowsTouches()){
				Vec2 newWorldPos = convertToWorldSpace(getPosition());
				const static float kMenuMinMove = 2;	//kMenuMinMove是一个冗余量，避免手抖按不上的问题
				if (fabs(newWorldPos.x - m_touchBeginWorldPos.x) > kMenuMinMove || fabs(newWorldPos.y - m_touchBeginWorldPos.y) > kMenuMinMove){
					break;
				}
			}
			_selectedItem->activate();
		}
		while (false);
	}
	_state = Menu::State::WAITING;
	this->release();
}

void DBMenu::onTouchCancelled(Touch* touch, Event* event)
{
	CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
	this->retain();
	if (_selectedItem)
	{
		_selectedItem->unselected();
	}
	_state = Menu::State::WAITING;
	this->release();
}

void DBMenu::onTouchMoved(Touch* touch, Event* event)
{
	CCASSERT(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
	MenuItem *currentItem = this->getItemForTouch(touch);
	if (currentItem != _selectedItem)
	{
		if (_selectedItem)
		{
			_selectedItem->unselected();
		}
		_selectedItem = currentItem;
		if (_selectedItem)
		{
			_selectedItem->selected();
		}
	}
}
/*menuitem可视区域外被触发的问题*/
void DBMenu::setCheckScissor(bool isCheckScissor){
	m_bCheckScissor = isCheckScissor;
}