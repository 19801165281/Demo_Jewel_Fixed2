#pragma once
#include "QNumber.h"
#define BITSNUM 6

class ScoreBoard:
	public Node
{
public:
	QNumber* score_bits[BITSNUM];//计分板暂时设为六位数
	ScoreBoard();
	~ScoreBoard();
	//根据新旧分数更新计分板显示
	void UpdateBoard(int old_score, int new_score);
};

