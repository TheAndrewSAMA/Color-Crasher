#include "AppDelegate.h"
#include "MainScene.h"
#include "Define.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	if (!glview) {
		glview = GLViewImpl::create(GAME_TITLE);
		glview->setFrameSize(FRAME_WIDTH, FRAME_HEIGHT);
		glview->setDesignResolutionSize(FRAME_WIDTH, FRAME_HEIGHT, ResolutionPolicy::SHOW_ALL);
		director->setOpenGLView(glview);
	}

	auto scene = MainScene::createScene();
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}
