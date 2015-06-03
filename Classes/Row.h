//	Created by yuerain
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class Row : public ui::Widget
{
public:
	Row();
	~Row();

	static Row* createWithName(const std::string& name);
	bool initWithName(const std::string& name);

	void checkBoxEvent(Ref* pSender, ui::CheckBox::EventType type);
private:

};
