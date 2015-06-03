#include "HelloWorldScene.h"
#include "MainLayer.h"
#include "FilterSprite.h"
#include "SpriteWithHue.h"
#include "ShaderEffect.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "vld.h"
#endif

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
	//����ڰ׻� ״̬
	/*auto sprite = FilterSprite::create("alien.png");
	GLfloat  filterMat[16] = {
		0.5f, 0.5f, 0.5f, -0.2f,
		0.5f, 0.5f, 0.5f, 0.2f,
		0.5f, 0.5f, 0.5f, -0.2f,
		0.0f, 0.1f, 0.0f, 1.0f
	};
	sprite->setFilterMat(FilterSprite::ES_BLACK_AND_WHITE);*/

	//���黻��ɫ ��װ �޸�ɫ��ʵ����Դ����
	//auto sprite = SpriteWithHue::create("alien.png");
	//sprite->setHue(2 * M_PI * 0.4);

	auto sprite =  EffectSprite::create("alien.png");
	sprite->setEffect(EffectGrey::create());
	
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	//����ͼƬ
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("plist/head_all.plist");
	cache->addSpriteFramesWithFile("plist/message_all.plist");
	cache->addSpriteFramesWithFile("plist/dialog_all.plist");
	

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    //Director::getInstance()->end();
	Director::getInstance()->replaceScene(MainLayer::createScene());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
