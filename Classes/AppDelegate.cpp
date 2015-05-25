#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

#define DB "data.db"
#define ALL_BOY "all_boy.csv"
#define ALL_GIRL "all_girl.csv"
#define BOY_DIALOG "boy_dialog.csv"
#define GIRL_DIALOG "girl_dialog.csv"

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}
// Android or iOS
bool isFileExist(const char* pFileName)
{
	if (!pFileName)return false;
	std::string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += pFileName;
	FILE *pFp = fopen(filePath.c_str(), "r");
	log(filePath.c_str());
	if (pFp)
	{
		fclose(pFp);
		return true;
	}
	return false;
}

void copyData(const char* pFileName)
{
	std::string strPath = FileUtils::getInstance()->fullPathForFilename(pFileName);
	ssize_t len = 0;
	unsigned char* data = NULL;
	data = FileUtils::getInstance()->getFileData(strPath.c_str(), "r", &len);

	std::string destPath = FileUtils::getInstance()->getWritablePath();
	destPath += pFileName;

	FILE *pFp = fopen(destPath.c_str(), "w+");
	fwrite(data, sizeof(char), len, pFp);
	fclose(pFp);
	delete[]data;
	data = NULL;
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("NoNameGame", Rect(0, 0, 320, 568));
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(640, 1136, ResolutionPolicy::FIXED_WIDTH);
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

	//存储所有文件的
	std::vector<std::string> allfile = { DB, ALL_BOY };
	//拷贝文件
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)//Android下需要复制数据文件
	for (int i = 0; i < allfile.size(); i++){
		if (isFileExist(allfile.at(i).c_str()) == false){	//检查文件是否存在 不存在的话 拷贝一份
			copyData(allfile.at(i).c_str());
		}
	}
#endif

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
