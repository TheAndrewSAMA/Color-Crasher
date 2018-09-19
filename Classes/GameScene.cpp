#include "GameScene.h"
#include "GameOverScene.h"
#include "Define.h"
#include <random>
#include <time.h>
#include <fstream>

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init() {

	if (!Layer::init())
	{
		return false;
	}

	// Rand seed initialization
	srand(time(NULL));

	// Background sprite definition and positioning
	auto bg = Sprite::create(BG_FILE);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(0, 0);
	this->addChild(bg);

	// scoreLabel sprite definition and positioning
	auto scoreBox = Sprite::create(SCORE_BOX_FILE);
	scoreBox->setAnchorPoint(Vec2(1, 1));
	scoreBox->setPosition(Director::getInstance()->getVisibleSize().width - 5, Director::getInstance()->getVisibleSize().height - 5);
	this->addChild(scoreBox);

	// scoreLabel label creation and positioning
	scoreLabel = Label::createWithTTF(std::to_string(scoreValue), FONT, FONT_SIZE - 10);
	scoreLabel->setAnchorPoint(Vec2(0, 0));
	scoreLabel->setPosition(15, 5);
	scoreLabel->setTextColor(Color4B::WHITE);
	scoreBox->addChild(scoreLabel);

	// Time sprite creation and positioning
	auto timeBox = Sprite::create(TIME_BOX_FILE);
	timeBox->setAnchorPoint(Vec2(0, 1));
	timeBox->setPosition(5, Director::getInstance()->getVisibleSize().height - 5);
	this->addChild(timeBox);

	// Countdown label creation and positioning
	std::string minutes = std::to_string(countdown / 60);
	std::string seconds = std::to_string(countdown % 60);
	if (countdown % 60 < 10) {
		seconds = "0" + seconds;
	}
	timeLabel = Label::createWithTTF(minutes + ":" + seconds, FONT, FONT_SIZE - 10);
	timeLabel->setAnchorPoint(Vec2(0, 0));
	timeLabel->setPosition(15, 5);
	timeLabel->setTextColor(Color4B::WHITE);
	timeBox->addChild(timeLabel);

	// Tiles creation and positioning
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

			grid[x][y] = Sprite::create(TILE_FILE);
			setRandColor(grid[x][y]);
			grid[x][y]->setAnchorPoint(Vec2(0, 0));
			grid[x][y]->setPosition(posX, posY);
			// The next tile will be "TILE_GAP" pixels above
			posY += TILE_SIZE + TILE_GAP;
			this->addChild(grid[x][y]);

			auto touchListener = EventListenerTouchOneByOne::create();

			touchListener->onTouchBegan = [=](Touch* touch, Event* event) {

				auto bounds = event->getCurrentTarget()->getBoundingBox();

				if (bounds.containsPoint(touch->getLocation())) {

					auto pause = DelayTime::create(1.0f);
					auto blocks = CallFunc::create([=](){
						if (checkBlocks(grid)){
							writeScore();
							auto scene = GameOverScene::createScene(NO_BLOCKS_REASON, scoreValue);
							Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(48,46,91)));
						}
					});
					seekAndDestroy(grid, x, y);
					updateScore(scoreLabel);
					updateTime(timeLabel);
					explodedTiles = 0;
					this->runAction(Sequence::create(pause, blocks, NULL));

				}
				return true;

			};

			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, grid[x][y]);

		}
		// Return to the first line
		posY = GRID_Y;
		// Move to the next column
		posX += TILE_SIZE + TILE_GAP;
	}

	this->schedule(schedule_selector(GameScene::updateTimer), 1.0);

	return true;

}

void GameScene::updateTimer(float dt) {
	countdown -= 1;
	if (countdown == 0){
		writeScore();
		auto scene = GameOverScene::createScene(NO_TIME_REASON, scoreValue);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(48,46,91)));
	}
	std::string minutes = std::to_string(countdown / 60);
	std::string seconds = std::to_string(countdown % 60);
	if (countdown % 60 < 10) {
		seconds = "0" + seconds;
	}
	timeLabel->setString(minutes + ":" + seconds);
}

bool GameScene::hasSameColor(Sprite* s1, Sprite* s2) {
	if (s1->getColor() == s2->getColor())
		return true;
	else
		return false;
}

void GameScene::setRandColor(Sprite* s) {
	auto color = s->getColor();
	switch (rand() % 6)
	{
	case 0:
		s->setColor(Color3B::RED);
		break;

	case 1:
		s->setColor(Color3B::BLUE);
		break;

	case 2:
		s->setColor(Color3B::GREEN);
		break;

	case 3:
		s->setColor(Color3B::YELLOW);
		break;

	case 4:
		s->setColor(Color3B(238, 130, 238));
		break;

	case 5:
		s->setColor(Color3B::ORANGE);
		break;
	}
	if (color == s->getColor())
		setRandColor(s);
}

bool GameScene::hasLeftTwin(Sprite* grid[8][8], int x, int y) {
	if (x > 0) {
		if (hasSameColor(grid[x][y], grid[x - 1][y]))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool GameScene::hasRightTwin(Sprite* grid[8][8], int x, int y) {
	if (x < 7) {
		if (hasSameColor(grid[x][y], grid[x + 1][y]))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool GameScene::hasAboveTwin(Sprite* grid[8][8], int x, int y) {
	if (y < 7) {
		if (hasSameColor(grid[x][y], grid[x][y+1]))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool GameScene::hasBelowTwin(Sprite* grid[8][8], int x, int y) {
	if (y > 0) {
		if (hasSameColor(grid[x][y], grid[x][y-1]))
			return true;
		else
			return false;
	}
	else
		return false;
}

void GameScene::updateScore(Label* scoreLabel){
	if (explodedTiles == 0)
		return;
	int scoreGain = (explodedTiles - 1) * 80 + ((explodedTiles-2)/5) * ((explodedTiles-2)/5);
	scoreValue += scoreGain;
	scoreLabel->setString(std::to_string(scoreValue));
}

void GameScene::updateTime(Label* timeLabel){
	if (explodedTiles == 0)
		return;
	int timeGained = 10 + ((explodedTiles-2)/3) * ((explodedTiles-2)/3) * 20;
	countdown += timeGained;
	std::string minutes = std::to_string(countdown / 60);
	std::string seconds = std::to_string(countdown % 60);
	if (countdown % 60 < 10) {
		seconds = "0" + seconds;
	}
	timeLabel->setString(minutes + ":" + seconds);
}

void GameScene::seekAndDestroy(Sprite* s[8][8], int x, int y) {
	auto fadeIn = FadeIn::create(0.1f);
  auto fadeOut = FadeOut::create(0.1f);
	auto pause = DelayTime::create(0.1f);
	auto randColor = CallFunc::create([=](){setRandColor(s[x][y]);});
	if (hasLeftTwin(s,x,y) || hasAboveTwin(s,x,y) || hasRightTwin(s,x,y) || hasBelowTwin(s,x,y)){
		if (hasLeftTwin(s,x,y) && turnR == 0){
			turnL ++;
			seekAndDestroy(s,x-1,y);
			turnL --;
		}
		if (hasAboveTwin(s,x,y) && turnB == 0){
			turnA ++;
			seekAndDestroy(s,x,y+1);
			turnA --;
		}
		if (hasRightTwin(s,x,y) && turnL == 0){
			turnR ++;
			seekAndDestroy(s,x+1,y);
			turnR --;
		}
		if (hasBelowTwin(s,x,y) && turnA == 0){
			turnB ++;
			seekAndDestroy(s,x,y-1);
			turnB --;
		}
		s[x][y]->runAction(Sequence::create(fadeOut, pause, randColor, fadeIn, NULL));
		explodedTiles++;
	}
}

bool GameScene::checkBlocks(Sprite* s[8][8]){
	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 8; y++){
			if (hasLeftTwin(s,x,y) || hasAboveTwin(s,x,y) || hasRightTwin(s,x,y) || hasBelowTwin(s,x,y))
				return false;
		}
	}
	return true;
}

int GameScene::readScore(){
	int score;
	std::ifstream scoreFile;
	scoreFile.open (SCORE_FILE);
	scoreFile >> score;
	scoreFile.close();
	return score;
}

void GameScene::writeScore(){
	if (readScore() < scoreValue){
		std::ofstream scoreFile;
		scoreFile.open (SCORE_FILE);
		scoreFile << scoreValue;
		scoreFile.close();
	}
}
