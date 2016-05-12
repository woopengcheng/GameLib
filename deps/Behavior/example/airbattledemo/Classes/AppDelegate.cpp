#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::createWithRect("AirBattleDemo", Rect(0, 0, 540, 960)); //设置PC端的设备分辨率，窗口名。在移动端这行代码无效。
        director->setOpenGLView(glview);
    }

	glview->setDesignResolutionSize(540, 960, ResolutionPolicy::SHOW_ALL); //设置游戏分辨率，尚未适配移动设备屏幕。

    director->setDisplayStats(true);

    director->setAnimationInterval(1.0 / 60);

    auto scene = GameScene::createScene();

    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
