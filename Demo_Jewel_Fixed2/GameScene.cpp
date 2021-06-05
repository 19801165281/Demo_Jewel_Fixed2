#include "Jewel.h"
#include "GameScene.h"
#include <vector>
#include<windows.h>
#include <thread>
#include <mutex>
#include <windows.h>
using namespace std;


MatrixNode* p0;//���ڲ��񶯻�״̬����
int GameScene::selected_jewels_numbers = 0;
Jewel* GameScene::map[8][8];
bool GameScene::canResume = true;

GameScene::GameScene()
{
	gt = new GameTask(1);//Ĭ��Ϊ��һ��
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

	int old_score = 0;		//���ڲ���ɵķ���
	int new_score = 0;		//���ڲ����µķ���
	//if (����ͣ)
	if (GameScene::selected_jewels_numbers == 2)// ���ѡ��2������
	{
		int a = 0, b = 0, flag = 0;
		int i, j;
		Jewel* jew1 = NULL;
		Jewel* jew2 = NULL;

		//�Ӳ���

		//step++


		//�ҵ�������ѡ�еı�ʯ
		for (i = 0; i < 8; i++) {//��һ����ʯ
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

		for (i = 0; i < 8; i++) {//�ڶ�����ʯ
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

		//��������������
		if (((a == i) && ((b == j + 1) || (b == j - 1)) || ((b == j) && ((a == i + 1) || (a == i - 1))))) {

			int pos[4] = { a, b, i, j };
			old_score = this->gt->getScore();
			MatrixNode* StatusSet = this->gt->breakTask(pos);
			int lenght = StatusSet->lenght();
			if (StatusSet)//����������Բ�������
			{
				jew1->isSelected = false;
				jew2->isSelected = false;
				//�����ϵĽ���
				Jewel::Exchange(jew1, jew2, 0, false);//��������
				GameScene::map[i][j] = jew1;
				GameScene::map[a][b] = jew2;
				//ִ��״̬�����еĽڵ㶯��
				p0 = StatusSet;//����״̬����
				new_score = this->gt->getScore();//����ɷ���
			}
			else {
				//����������ܲ������ӣ����Ŷ���
				// ִ��˳�򶯻�
				// ִ��˳�򶯻�
				Jewel::Exchange(jew1, jew2, 0, true);//��������
			}
			printf("The lenght of the set is %d\n", lenght);
		}
		else
		{
			//�����ڵı�ʯ��ѡ��ȡ��ѡ��״̬
			jew1->Select();
			jew2->Select();
			jew1->isSelected = false;
			jew2->isSelected = false;
		}
		init_selected_jewels_numbers();//��ʼ��ѡ�б�ʯ��Ŀ
	}
	if (p0 && GameScene::canResume)//canResume������Ƕ��������ĵ㾦֮�ʣ�������һ�������Ϳ��Կ���ÿ�ֶ���֮���ʱ����
	{
		GameScene::canResume = false;
		//ͳ��ÿһ���������
		int* empty_in_col;
		int* fall_start_from;
		empty_in_col = (int*)malloc(sizeof(int) * MAPCOLNUM);
		fall_start_from = (int*)malloc(sizeof(int) * MAPCOLNUM);
		if (!empty_in_col || !fall_start_from) exit(0);
		memset(empty_in_col, 0, sizeof(int) * MAPCOLNUM);
		memset(fall_start_from, 255, sizeof(int) * MAPCOLNUM);
		//������
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
		//���½�
		MatrixNode* q0 = p0;
		p0 = p0->next;
		for (int j = 0; j < MAPCOLNUM; j++)
		{
			if (empty_in_col[j] == 0)
			{
				continue;
			}
			//���ڽ��ڵı�ʯ�½�����Ӧλ��
			if (fall_start_from[j] >= 0)
			{
				//ͳ�Ʊ���������0����һ��
				int last_zero_row = MAPCOLNUM - 1;
				for (int i = MAPCOLNUM - 1; i >= 0; i--)
				{
					if (q0->map[i][j] == 0)
					{
						last_zero_row = i;
						break;
					}
				}
				//�����һ�������ң���������Ҹ������һ�ŵ�����������
				//�������½������һ���������У��ҽ����һ��������������
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
			//�´��������ɵı�ʯ�����ŵ���Ӧ���Ϸ�
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

void GameScene::Initialize()
{
	//��Ϸ����
	auto background = gcnew Sprite(L"res/BG03.png");

	background->setPos(Window::getWidth() / 2, Window::getHeight() / 2);
	this->addChild(background);
	//����

	auto chessboard = gcnew Sprite(L"res/chessboard.png");
	chessboard->setAnchor(0.0f, 0.0f);
	chessboard->setPos(414, 30);
	this->addChild(chessboard);

	//������ʯ
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			auto jew = new Jewel(this->gt->initState()[i][j]);//����map���ɲ�ͬͼ��ͼƬ
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

	//��ʾ��ť
	this->hint_btn->setPos(200, 550);
	this->hint_btn->setVisible(true);
	//������ʾ��ť�ص�����
	this->hint_btn->setClickFunc([&]() {
		//��ȡ���Ե�����������ʯλ��
		int *idea = gt->getHint();
		int row1 = idea[0];
		int col1 = idea[1];
		int row2 = idea[2];
		int col2 = idea[3];
		//��ȡ���Ե�����������ʯ
		Jewel* jew1 = GameScene::map[row1][col1];
		Jewel* jew2 = GameScene::map[row2][col2];
		//��ȡ���Ե�����������ʯ��λ��
		float x1 = jew1->getPosX();
		float y1 = jew1->getPosY();
		float x2 = jew2->getPosX();
		float y2 = jew2->getPosY();
		//����һ��С�ֶ�����ʹ���һ��λ��Ų����һ��λ��
		auto hint_hand = gcnew Sprite(L"res\\hint_hand.png");	//����С��
		//С�ָ����ڵ�һ�ű�ʯ�Ķ���
		hint_hand->setPos(x1, y1);
		this->addChild(hint_hand);
		auto hand_appear = gcnew FadeIn(0.5f);
		//С��Ų�����ڶ��ű�ʯ�Ķ���
		auto hand_move = gcnew MoveTo(0.5f, Point(x2, y2));
		//С����ʧ�ڵڶ��ű�ʯ�Ķ���
		auto hand_disapper = gcnew FadeOut(0.5f);
		//ִ�ж�������
		hint_hand->runAction(gcnew Sequence({hand_appear, hand_move, hand_disapper}));
		});
	this->addChild(this->hint_btn);

	//�Ʒְ�
	this->score_board->setPos(30, 35);
	this->score_board->setVisible(true);
	this->addChild(this->score_board);
}
