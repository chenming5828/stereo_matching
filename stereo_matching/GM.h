#pragma once
#include "Solver.h"


class GM : public Solver
{
public:
	GM(unsigned int img_w, unsigned int img_h);
	virtual void Process(Mat &ll, Mat &rr);
	virtual ~GM();
};

