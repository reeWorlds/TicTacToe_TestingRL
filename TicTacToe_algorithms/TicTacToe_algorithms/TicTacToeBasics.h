#pragma once

#include "includes.h"

constexpr int N_States = 19683;

class Matrix
{
public:
	
	int a[3][3];

	Matrix();
	Matrix(int hash);
	Matrix(const Matrix& other);

	void initZero();
	void initByHash(int hash);

	int isFinished();

	int checkState();

	int getHash();
};