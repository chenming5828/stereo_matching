#pragma once

#include "Solver.h"


class GM : public Solver
{
public:
	GM(Mat &ll, Mat &rr);
	virtual void Process();
	~GM();
};

