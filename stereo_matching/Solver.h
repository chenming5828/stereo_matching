#pragma once

#include "global.h"
#include "cost.h"


const uchar MAX_DISP = 128;
const uchar INVALID_DISP = MAX_DISP + 1;
const uchar WIN_H = 5;
const uchar WIN_W = 5;
const float UNIQUE_RATIO = 0.8;


class Solver
{
public:
	Solver();
	Solver(Mat &ll, Mat &rr);
	void Show_disp(); 
	virtual void Process();
	void Build_dsi();
	virtual ~Solver();

protected:
	Mat ll, rr;
	Mat disp, disp_float, disp_float_colored;
	uint16_t img_w, img_h;
	float *cost;
};

