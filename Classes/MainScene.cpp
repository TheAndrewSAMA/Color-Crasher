#include "MainScene.h"
#include "GameScene.h"
#include "Define.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Sprites creation

	auto bg = Sprite::create(BG_FILE);
	auto logo = Sprite::create(LOGO_FILE);
	auto start = Sprite::create(START_FILE);

	// Setting sprites anchor points

	bg->setAnchorPoint(Vec2(0, 0));
	logo->setAnchorPoint(Vec2(0.5, 1));

	// Setting sprites position

	bg->setPosition(0, 0);
	logo->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height - 20);
	start->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);

	// Creating touch listener

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [](Touch* touch, Event* event) {

		auto bounds = event->getCurrentTarget()->getBoundingBox();

		if (bounds.containsPoint(touch->getLocation())) {

			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(48,46,91)));

		}
		return true;
	};

	// Sprites and listener loading

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, start);
	this->addChild(bg);
	this->addChild(logo);
	this->addChild(start);

	return true;
}
