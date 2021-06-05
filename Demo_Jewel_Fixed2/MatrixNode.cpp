#include "MatrixNode.h"
#include <iostream>

MatrixNode::MatrixNode()
{
	this->map = (int**)malloc(sizeof(int*) * MAPROWNUM);
	if (!this->map) exit(0);
	for (int i = 0; i < MAPCOLNUM; i++)
	{
		this->map[i] = (int*)malloc(sizeof(int) * MAPCOLNUM);
		if (!this->map[i]) exit(0);
	}
	this->next = nullptr;
}

int MatrixNode::lenght()
{
	MatrixNode* p0 = this;
	int lenght = 0;
	while (p0)
	{
		lenght++;
		p0 = p0->next;
	}
	return lenght;
}
