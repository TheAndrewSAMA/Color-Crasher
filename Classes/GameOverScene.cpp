#include "GameOverScene.h"
#include "GameScene.h"
#include "fstream"
#include "Define.h"

USING_NS_CC;

Scene* GameOverScene::createScene(std::string reason, int scoreValue)
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();

	//  Reason and Your Score label creation and positioning
	auto reasonLabel = Label::createWithTTF(reason, FONT, FONT_SIZE + 5);
	auto yourScore = Label::createWithTTF(YOUR_SCORE_LABEL + std::to_string(scoreValue), FONT, FONT_SIZE + 5 );
	yourScore->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 4);
	reasonLabel->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 6 * 4);

	// Rason and Your Score loading
	layer->addChild(reasonLabel);
	layer->addChild(yourScore);

	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Sprites creation

	auto bg = Sprite::create(BG_FILE);
	auto retry = Sprite::create(RETRY_FILE);
	auto gameOver = Label::createWithTTF(GAMEOVER_LABEL, FONT, FONT_SIZE + 10);
	auto bestScore = Label::createWithTTF(BESTSCORE_LABEL + std::to_string(readScore()), FONT, FONT_SIZE + 5);


	// Setting sprites reference coordinates

	bg->setAnchorPoint(Vec2(0, 0));

	// Setting sprites positions

	bg->setPosition(0, 0);
	retry->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	gameOver->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 4 * 3);
	bestScore->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 4 - 50);

	//Creating touch listener

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = [](Touch* touch, Event* event) {

		auto bounds = event->getCurrentTarget()->getBoundingBox();

		if (bounds.containsPoint(touch->getLocation())) {

			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(48,46,91)));

		}
		return true;
	};

	// Sprites loading

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, retry);
	this->addChild(bg);
	this->addChild(retry);
	this->addChild(gameOver);
	this->addChild(bestScore);

	return true;
}

int GameOverScene::readScore(){
	int score;
	std::ifstream scoreFile;
	scoreFile.open (SCORE_FILE);
	scoreFile >> score;
	scoreFile.close();
	return score;
}
