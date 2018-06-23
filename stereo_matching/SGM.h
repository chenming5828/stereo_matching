#pragma once

#include "Solver.h"


class SGM : public Solver
{
public:
	SGM(Mat &ll, Mat &rr);
	virtual void Process();
	virtual ~SGM();

private:
	float *cost, *L1, *L2, *L3, *L4, *sum_of_cost;
	float *min_L1, *min_L2, *min_L3, *min_L4;
	uint16_t P1, P2;
};

