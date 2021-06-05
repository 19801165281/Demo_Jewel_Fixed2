#pragma once
#include <easy2d/easy2d.h>

using namespace easy2d;
class QNumber:
	public Node
{
public:
	QNumber();
	void update(int num);
	~QNumber();
	int pic_weight;
	int pic_height;
};

