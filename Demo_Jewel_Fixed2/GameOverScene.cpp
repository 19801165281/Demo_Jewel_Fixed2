#include "GameOverScene.h"
#include "GameMask.h"
#include "GameScene.h"

GameOverScene::GameOverScene()
{

}

GameOverScene::GameOverScene(int time_used, int steps_used, int score, GameStatus& gst, bool next_allow)
{
	//蒙版背景图片
	auto game_mask = gcnew GameMask();
	this->addChild(game_mask);
	//结算界面文本显示信息处理
	//如果time_used是-1说明是经典模式的结算界面，不做显示处理
	//结算界面功能按钮

	//重开本局游戏
	//重开
	auto back_norm = gcnew Sprite(L"res\\back.png");
	auto back_selected = gcnew Sprite(L"res\\back.png");
	auto back_btn = gcnew Button(back_norm, back_selected);
	back_btn->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
	BtnDeal(back_norm, back_selected);
	back_btn->setClickFunc([&]() {
		Game::resume();
		gst.setGst(GStatus::Wash);
		SceneManager::back(gcnew FadeTransition(0.6f));
		});
	this->addChild(back_btn);
	//返回主菜单
	auto home_norm = gcnew Sprite(L"res\\home.png");
	auto home_selected = gcnew Sprite(L"res\\home.png");
	auto home_btn = gcnew Button(home_norm, home_selected);
	home_btn->setPos(Window::getWidth() / 2 - 200, Window::getHeight() / 2);
	this->BtnDeal(home_norm, home_selected);
	home_btn->setClickFunc([&]() {
		Game::resume();
		SceneManager::back(gcnew FadeTransition(0.6f));
		});
	this->addChild(home_btn);
	//进入下一关，进不去就禁用
	auto next_norm = gcnew Sprite(L"res\\next_allow.png");
	auto next_selected = gcnew Sprite(L"res\\next_allow.png");
	this->BtnDeal(next_norm, next_selected);
	auto next_btn = gcnew Button();
	next_btn->setEnable(next_allow);
	next_btn->setNormal(next_norm);
	next_btn->setSelected(next_selected);
	next_btn->setDisabled(gcnew Sprite(L"res\\next_disabled.png"));
	next_btn->setPos(Window::getWidth() / 2 + 200, Window::getHeight() / 2);
	next_btn->setClickFunc([&]() {
		Game::resume();
		GameScene* gs = new GameScene(6, Mode::Fast);
		SceneManager::enter(gs, gcnew FadeTransition(0.6f), false);
		});
	this->addChild(next_btn);
}

void GameOverScene::BtnDeal(Sprite* pic_1, Sprite* pic_2)
{
	pic_1->setScale(1.0f);
	pic_2->setScale(0.7f);
	pic_2->movePosY((pic_1->getHeight() - pic_2->getHeight()) / 2);
	pic_2->movePosX((pic_1->getWidth() - pic_2->getWidth()) / 2);
}

GameOverScene::~GameOverScene()
{

}
