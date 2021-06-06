#include "Jewel.h"
#include "GameScene.h"
#include <vector>
#include<windows.h>
#include <thread>
#include <mutex>
#include <windows.h>
using namespace std;


MatrixNode* p0;//用于捕获动画状态链表
int GameScene::selected_jewels_numbers = 0;
Jewel* GameScene::map[8][8];
bool GameScene::canResume = true;

GameScene::GameScene()
{
	gt = new GameTask(1);//默认为第一关
	this->hint_btn = gcnew HintButton();
	this->score_board = gcnew ScoreBoard();
	Initialize();
}

GameScene::GameScene(int l)
{
	gt = new GameTask(l);
	this->hint_btn = gcnew HintButton();
	this->score_board = gcnew ScoreBoard();
	Initialize();
}

void GameScene::onUpdate() {

	int old_score = 0;		//用于捕获旧的分数
	int new_score = 0;		//用于捕获新的分数
	//if (非暂停)
	//如果是死局
	if (this->gt->getDeadlock())
	{
		printf("Deadlock");
		this->WashJews();
	}
	if (GameScene::selected_jewels_numbers == 2)// 如果选中2个棋子
	{
		int a = 0, b = 0, flag = 0;
		int i, j;
		Jewel* jew1 = NULL;
		Jewel* jew2 = NULL;

		//加步数

		//step++


		//找到两个被选中的宝石
		for (i = 0; i < 8; i++) {//第一个宝石
			for (j = 0; j < 8; j++) {
				if (GameScene::map[i][j]->isSelected) {
					a = i;
					b = j;
					jew1 = GameScene::map[i][j];
					flag = 1;
					break;
				}
			}
			if (flag == 1) {
				break;
			}
		}

		flag = 0;

		for (i = 0; i < 8; i++) {//第二个宝石
			for (j = 0; j < 8; j++) {
				if (i == a && j == b) continue;
				if (GameScene::map[i][j]->isSelected) {
					jew2 = map[i][j];
					flag = 1;
					break;
				}
			}
			if (flag == 1) break;
		}

		//若两个棋子相邻
		if (((a == i) && ((b == j + 1) || (b == j - 1)) || ((b == j) && ((a == i + 1) || (a == i - 1))))) {

			int pos[4] = { a, b, i, j };
			old_score = this->gt->getScore();
			MatrixNode* StatusSet = this->gt->breakTask(pos);
			int lenght = StatusSet->lenght();
			if (StatusSet)//如果交换可以产生消子
			{
				jew1->isSelected = false;
				jew2->isSelected = false;
				//动画上的交换
				Jewel::Exchange(jew1, jew2, 0, false);//交换动画
				GameScene::map[i][j] = jew1;
				GameScene::map[a][b] = jew2;
				//执行状态链表中的节点动画
				p0 = StatusSet;//捕获状态链表
				new_score = this->gt->getScore();//捕获旧分数
			}
			else {
				//交换如果不能产生消子，播放动画
				// 执行顺序动画
				// 执行顺序动画
				Jewel::Exchange(jew1, jew2, 0, true);//交换动画
			}
			printf("The lenght of the set is %d\n", lenght);
		}
		else
		{
			//不相邻的宝石被选中取消选中状态
			jew1->Select();
			jew2->Select();
			jew1->isSelected = false;
			jew2->isSelected = false;
		}
		init_selected_jewels_numbers();//初始化选中宝石数目
	}
	if (p0 && GameScene::canResume)//canResume的设计是动画处理的点睛之笔，仅仅用一个变量就可以控制每轮动画之间的时间间隔
	{
		GameScene::canResume = false;
		//统计每一列下落情况
		int* empty_in_col;
		int* fall_start_from;
		empty_in_col = (int*)malloc(sizeof(int) * MAPCOLNUM);
		fall_start_from = (int*)malloc(sizeof(int) * MAPCOLNUM);
		if (!empty_in_col || !fall_start_from) exit(0);
		memset(empty_in_col, 0, sizeof(int) * MAPCOLNUM);
		memset(fall_start_from, 255, sizeof(int) * MAPCOLNUM);
		//先消子
		for (int j = 0; j < MAPCOLNUM; j++)
		{
			for (int i = MAPROWNUM - 1; i >= 0; i--)
			{
				if (p0->map[i][j] == 0)
				{
					GameScene::map[i][j]->Break();
					GameScene::map[i][j] = nullptr;
					empty_in_col[j]++;
				}
				else
				{
					fall_start_from[j] = i;
				}
			}
		}
		//再下降
		MatrixNode* q0 = p0;
		p0 = p0->next;
		for (int j = 0; j < MAPCOLNUM; j++)
		{
			if (empty_in_col[j] == 0)
			{
				continue;
			}
			//将在界内的宝石下降到对应位置
			if (fall_start_from[j] >= 0)
			{
				//统计本列中最靠后的0在哪一排
				int last_zero_row = MAPCOLNUM - 1;
				for (int i = MAPCOLNUM - 1; i >= 0; i--)
				{
					if (q0->map[i][j] == 0)
					{
						last_zero_row = i;
						break;
					}
				}
				//从最后一排向上找，如果非零且高于最后一排的零所在行数
				//则让其下降到最后一排零所在行，且将最后一排所在行数更新
				for (int i = MAPCOLNUM - 1; i >= 0; i--)
				{
					if (q0->map[i][j] != 0 && i < last_zero_row)
					{
						GameScene::map[i][j]->Fall(last_zero_row - i);
						GameScene::map[last_zero_row][j] = GameScene::map[i][j];
						GameScene::map[i][j] = nullptr;
						last_zero_row--;
					}
				}
			}
			//新创建新生成的宝石，安排到对应列上方
			for (int k = 0; k < empty_in_col[j]; k++)
			{
				int jew_num = p0->map[empty_in_col[j] - k - 1][j];
				Jewel* new_jew = new Jewel(jew_num);
				new_jew->pos_row = 30.0f + 36.25f + 72.5f * (float)(-1);
				new_jew->pos_col = 414.0f + 36.25f + 72.5f * (float)(j);
				new_jew->setPosX(new_jew->pos_col);
				new_jew->setPosY(new_jew->pos_row);
				new_jew->setVisible(true);
				new_jew->setScale(0.8f);
				this->addChild(new_jew);
				new_jew->Fall(empty_in_col[j] - k);
				GameScene::map[empty_in_col[j] - k - 1][j] = new_jew;
			}
		}
		p0 = p0->next;
		if (!p0)
		{
			new_score = this->gt->getScore();
			this->score_board->UpdateBoard(old_score, new_score);
		}
	}
}

GameScene::~GameScene()
{

}

int GameScene::get_selected_jewels_numbers()
{
	return selected_jewels_numbers;
}

void GameScene::add_selected_jewels_numbers()
{
	selected_jewels_numbers++;
}

void GameScene::init_selected_jewels_numbers()
{
	selected_jewels_numbers = 0;
}

void GameScene::minu_selected_jewels_numbers()
{
	selected_jewels_numbers--;
}

void GameScene::WashJews()
{
	printf("Washing...\n");
	int** new_map = this->gt->getNewState();
	for (int i = MAPROWNUM - 1; i >= 0; i--)
	{
		for (int j = 0; j < MAPCOLNUM; j++)
		{
			GameScene::map[i][j]->Break();
			this->removeChild(GameScene::map[i][j]);
			GameScene::map[i][j] = nullptr;
			auto jew = new Jewel(new_map[i][j]);//根据map生成不同图像图片
			jew->pos_row = 30.0f + 36.25f + 72.5f * (float)(-1);
			jew->pos_col = 414.0f + 36.25f + 72.5f * (float)(j);
			jew->setScale(0.8f);
			jew->setPosX(jew->pos_col);
			jew->setPosY(jew->pos_row);
			jew->setVisible(true);
			this->addChild(jew);
			jew->Fall(i + 1);
			GameScene::map[i][j] = jew;
		}
	}
	printf("Washing OK!\n");
}

void GameScene::Initialize()
{
	//游戏背景
	auto background = gcnew Sprite(L"res/BG03.png");

	background->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
	this->addChild(background);
	//棋盘

	auto chessboard = gcnew Sprite(L"res/chessboard.png");
	chessboard->setAnchor(0.0f, 0.0f);
	chessboard->setPos(414, 30);
	this->addChild(chessboard);

	//创建宝石
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			auto jew = new Jewel(this->gt->initState()[i][j]);//根据map生成不同图像图片
			jew->pos_row = 30.0f + 36.25f + 72.5f * (float)(i);
			jew->pos_col = 414.0f + 36.25f + 72.5f * (float)(j);
			jew->setScale(0.8f);
			jew->setPosX(jew->pos_col);
			jew->setPosY(jew->pos_row);
			jew->setVisible(true);
			this->addChild(jew);
			GameScene::map[i][j] = jew;
		}
	}

	//提示按钮
	this->hint_btn->setPos(200, 550);
	this->hint_btn->setVisible(true);
	//设置提示按钮回调函数
	this->hint_btn->setClickFunc([&]() {
		if (this->gt->getDeadlock())
		{
			return;
		}
		//获取可以调换的两个宝石位置
		int *idea = gt->getHint();
		int row1 = idea[0];
		int col1 = idea[1];
		int row2 = idea[2];
		int col2 = idea[3];
		//获取可以调换的两个宝石
		Jewel* jew1 = GameScene::map[row1][col1];
		Jewel* jew2 = GameScene::map[row2][col2];
		//获取可以调换的两个宝石的位置
		float x1 = jew1->getPosX();
		float y1 = jew1->getPosY();
		float x2 = jew2->getPosX();
		float y2 = jew2->getPosY();
		//创建一个小手动画，使其从一个位置挪到另一个位置
		auto hint_hand = gcnew Sprite(L"res\\hint_hand.png");	//创建小手
		//小手浮现在第一颗宝石的动画
		hint_hand->setPos(x1, y1);
		this->addChild(hint_hand);
		auto hand_appear = gcnew FadeIn(0.5f);
		//小手挪动到第二颗宝石的动画
		auto hand_move = gcnew MoveTo(0.5f, Point(x2, y2));
		//小手消失在第二颗宝石的动画
		auto hand_disapper = gcnew FadeOut(0.5f);
		//执行动画序列
		hint_hand->runAction(gcnew Sequence({hand_appear, hand_move, hand_disapper}));
		});
	this->addChild(this->hint_btn);

	//计分板
	this->score_board->setPos(30, 35);
	this->score_board->setVisible(true);
	this->addChild(this->score_board);

	////洗牌按钮
	//WashButton *wb = gcnew WashButton();
	//wb->setPos(70, 135);
	//wb->setClickFunc([&]() {
	//	this->WashJews();
	//	});
	//this->addChild(wb);
}

