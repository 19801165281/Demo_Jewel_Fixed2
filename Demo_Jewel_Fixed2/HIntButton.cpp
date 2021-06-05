#include "HintButton.h"

HintButton::HintButton()//只设置样式，在GameScene内设置回调函数和位置
{
	auto hint_pic = gcnew Sprite(L"res\\hint.png");
	auto hint_pic_selected = gcnew Sprite(L"res\\hint.png");
	this->setNormal(hint_pic);
	this->setSelected(hint_pic_selected);
	hint_pic->setScale(0.5f);
	hint_pic_selected->setScale(0.3f);
	hint_pic_selected->movePosY((hint_pic->getHeight() - hint_pic_selected->getHeight()) / 2);
	hint_pic_selected->movePosX((hint_pic->getWidth() - hint_pic_selected->getWidth()) / 2);
}

HintButton::~HintButton()
{

}
