#pragma once

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(std::string reason, int scoreValue);
	virtual bool init() override;
	CREATE_FUNC(GameOverScene);
private:
	int readScore();
};
