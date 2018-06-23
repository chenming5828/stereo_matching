#pragma once

#include "Solver.h"


class SGM : public Solver
{
public:
	SGM(Mat &ll, Mat &rr);
	virtual void Process();
	virtual ~SGM();

private:
	float *cost, *L1, *sum_of_cost;
	uint16_t P1, P2;
};

