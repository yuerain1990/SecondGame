/**
 ��дMenu�ؼ�����ScrollView��
 */
#pragma  once
#include "cocos2d.h"

USING_NS_CC;

class DBMenu :public Menu
{
public:
	bool init();

	/** initializes a Menu with a NSArray of MenuItem objects */
	bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
	static DBMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
	static DBMenu* createWithItem(MenuItem* item);

	/** creates a Menu with MenuItem objects */
	static DBMenu* createWithItems(MenuItem *firstItem, va_list args);
	static DBMenu* create(MenuItem* item, ...) CC_REQUIRES_NULL_TERMINATION;

	virtual bool onTouchBegan(Touch* touch, Event* event) override;
	virtual void onTouchEnded(Touch* touch, Event* event) override;
	virtual void onTouchCancelled(Touch* touch, Event* event) override;
	virtual void onTouchMoved(Touch* touch, Event* event) override;
	//���menu item���������ⱻ����������
	void setCheckScissor(bool isCheckScissor);
private:
	Vec2 m_touchBeginWorldPos;
	bool m_bCheckScissor;
};