#pragma once
#include <easy2d/easy2d.h>

using namespace easy2d;

enum class GStatus {
	Playing,
	Pause,
	Wash
};

class GameStatus
{
public:
	GameStatus();
	~GameStatus();
	GStatus getGst();
	void setGst(GStatus gst);
private:
	GStatus gst;
};

