#pragma once
#include <easy2d/easy2d.h>
#include "GameMask.h"
#include "GameScene.h"

using namespace easy2d;
class SubMenu:
	public Scene
{
public:
	SubMenu(GameStatus &gst);
	~SubMenu();
	//GameStatus* gst;
private:
	void BtnDeal(Sprite* pic_1, Sprite* pic_2);
};

