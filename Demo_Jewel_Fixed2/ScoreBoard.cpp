#include "ScoreBoard.h"
#include <math.h>

ScoreBoard::ScoreBoard()
{
	//初始化计分板显示为000000
	for (int i = 0; i < BITSNUM; i++)
	{
		auto bit = gcnew QNumber();
		bit->setPosX((bit->pic_weight / 2.0f) + (bit->pic_weight * i));
		bit->setPosY((bit->pic_height) / 2.0f);
		this->score_bits[i] = bit;
		this->addChild(bit);
	}
}

ScoreBoard::~ScoreBoard()
{

}

void ScoreBoard::UpdateBoard(int old_score, int new_score)
{
	int divide_num = pow(10, BITSNUM - 1);
	for (int i = 0; i < BITSNUM; i++)
	{
		int old_bit = old_score / divide_num;
		int new_bit = new_score / divide_num;
		old_score -= old_bit * divide_num;
		new_score -= new_bit * divide_num;
		divide_num /= 10;
		if (old_bit != new_bit)		//旧位不等于新位，更新该位
		{
			this->score_bits[i]->update(new_bit);
		}
	}
}
