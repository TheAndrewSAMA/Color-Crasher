#pragma once

#include "cocos2d.h"
#include <string>

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(GameScene);
private:

	// Common parameters definition
	const int TILE_SIZE = 50;
	const int TILE_GAP = 5;

	// Grid reference coordinates
	const int GRID_X = 35;
	const int GRID_Y = 35;

	// Grid declaration
	cocos2d::Sprite* grid[8][8];

	// Labels declaration
	cocos2d::Label* timeLabel;
	cocos2d::Label* scoreLabel;

	// Grid coordinates
	int posX = GRID_X;
	int posY = GRID_Y;

	// Turn counter initialization
	int turnL = 0;
	int turnA = 0;
	int turnR = 0;
	int turnB = 0;

	// Exploded tiles counter initialization
	int explodedTiles = 0;

	// Countdown declaration
	int countdown = 120;

	// Score initialization
	int scoreValue = 0;

	// Automatic countdown label updater
	void updateTimer(float dt);

	// Function to compare the color of two tiles
	bool hasSameColor(cocos2d::Sprite* s1, cocos2d::Sprite* s2);

	// Function to set a random color on a tile
	void setRandColor(cocos2d::Sprite* s);

	// Function to check if a tile has a same colored tile on the left
	bool hasLeftTwin(cocos2d::Sprite* grid[8][8], int x, int y);

	// Function to check if a tile has a same colored tile on the right
	bool hasRightTwin(cocos2d::Sprite* grid[8][8], int x, int y);

	// Function to check if a tile has a same colored tile above
	bool hasAboveTwin(cocos2d::Sprite* grid[8][8], int x, int y);

	// Function to check if a tile has a same colored tile below
	bool hasBelowTwin(cocos2d::Sprite* grid[8][8], int x, int y);

	// Score updater
	void updateScore(cocos2d::Label* score);

  // Manual countdown label updater
	void updateTime(cocos2d::Label* timer);

	// Checks if a tile is part of a block and then "destroys" the block, replacing
	// the color of each tile inside it.
	void seekAndDestroy(cocos2d::Sprite* s[8][8], int x, int y);

	// Checks if there are other blocks of same colored tiles inside the grid
	bool checkBlocks(cocos2d::Sprite* s[8][8]);

	// Writes the actual score
	void writeScore();

	// Reads the best score
	int readScore();
};
