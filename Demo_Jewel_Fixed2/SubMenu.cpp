#include "SubMenu.h"

SubMenu::SubMenu(GameStatus &gst)
{
	//蒙版背景图片
	auto game_mask = gcnew GameMask();
	this->addChild(game_mask);
	//继续游戏
	auto resume_norm = gcnew Sprite(L"res\\resume.png");
	auto resume_selected = gcnew Sprite(L"res\\resume.png");
	auto resume_btn = gcnew Button(resume_norm, resume_selected);
	resume_btn->setPos(Window::getWidth() / 2 - 200, Window::getHeight() / 2);
	BtnDeal(resume_norm, resume_selected);
	resume_btn->setClickFunc([&]() {
		Game::resume();
		SceneManager::back(gcnew FadeTransition(0.6f));
		});
	this->addChild(resume_btn);

	//返回主菜单
	auto home_norm = gcnew Sprite(L"res\\home.png");
	auto home_selected = gcnew Sprite(L"res\\home.png");
	auto home_btn = gcnew Button(home_norm, home_selected);
	home_btn->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
	BtnDeal(home_norm, home_selected);
	home_btn->setClickFunc([&]() {
		Game::resume();
		SceneManager::back(gcnew FadeTransition(0.6f));
		});
	this->addChild(home_btn);
	
	//重开
	auto back_norm = gcnew Sprite(L"res\\back.png");
	auto back_selected = gcnew Sprite(L"res\\back.png");
	auto back_btn = gcnew Button(back_norm, back_selected);
	back_btn->setPos(Window::getWidth() / 2 + 200, Window::getHeight() / 2);
	BtnDeal(back_norm, back_selected);
	back_btn->setClickFunc([&]() {
		Game::resume();
		gst.setGst(GStatus::Wash);
		SceneManager::back(gcnew FadeTransition(0.6f));
		});
	this->addChild(back_btn);
}

void SubMenu::BtnDeal(Sprite *pic_1, Sprite *pic_2)
{
	pic_1->setScale(1.0f);
	pic_2->setScale(0.7f);
	pic_2->movePosY((pic_1->getHeight() - pic_2->getHeight()) / 2);
	pic_2->movePosX((pic_1->getWidth() - pic_2->getWidth()) / 2);
}

SubMenu::~SubMenu()
{
}
