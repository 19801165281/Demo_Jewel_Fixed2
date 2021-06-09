#pragma once
#include <easy2d/easy2d.h>
#include "GameStatus.h"

using namespace easy2d;

class GameOverScene:
	public Scene
{
public:
	GameOverScene();
	GameOverScene(int time_used, int steps_used, int score, GameStatus &gst, bool next_allow);
	void BtnDeal(Sprite* pic_1, Sprite* pic_2);
	~GameOverScene();
};

