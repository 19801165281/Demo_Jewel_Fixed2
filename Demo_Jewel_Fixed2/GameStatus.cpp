#include "GameStatus.h"

GameStatus::GameStatus()
{
	this->gst = GStatus::Playing;
}

GameStatus::~GameStatus()
{

}

GStatus GameStatus::getGst()
{
	return this->gst;
}

void GameStatus::setGst(GStatus gst)
{
	this->gst = gst;
}
