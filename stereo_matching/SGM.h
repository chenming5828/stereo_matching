#pragma once

#include "Solver.h"


const bool USE_8_PATH = 1;


class SGM : public Solver
{
public:
	SGM(Mat &ll, Mat &rr);
	virtual void Process();
	virtual ~SGM();

private:
	float *L1, *L2, *L3, *L4, *L5, *L6, *L7, *L8;
	float *min_L1, *min_L2, *min_L3, *min_L4, *min_L5, *min_L6, *min_L7, *min_L8;
	int P1, P2;
};

