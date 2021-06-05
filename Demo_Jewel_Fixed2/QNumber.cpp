#include "QNumber.h"
#include <string>

QNumber::QNumber()
{
	update(0);
}

void QNumber::update(int num)
{
	if (this->getChildrenCount() > 0)
	{
		this->removeAllChildren();
	}
	std::string path = "res\\Qnum_" + std::to_string(num) + ".png";
	String widename = NarrowToWide(path);
	auto num_pic = gcnew Sprite(widename);
	this->pic_weight = num_pic->getWidth();
	this->pic_height = num_pic->getHeight();
	this->addChild(num_pic);
}

QNumber::~QNumber()
{

}
