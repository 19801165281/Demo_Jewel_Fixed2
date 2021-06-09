#include "GameMask.h"

GameMask::GameMask()
{
	this->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
	auto mask_pic = gcnew Sprite(L"res\\game_mask.png");
	mask_pic->setPos(this->getWidth() / 2, this->getHeight() / 2);
	this->addChild(mask_pic);
}

GameMask::~GameMask()
{

}
