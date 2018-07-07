#pragma once

#include "global.h"
#include "cost.h"


const uchar MAX_DISP = 64;
const uchar INVALID_DISP = MAX_DISP + 1;
//const uchar INVALID_DISP = 0;
const uchar WIN_H = 5;
const uchar WIN_W = 5;


class Solver
{
public:
	Solver();
	Solver(Mat &ll, Mat &rr);
	void Show_disp(); 
	virtual void Process();

protected:
	Mat ll, rr;
	Mat disp, disp_float, disp_float_colored;
	uint16_t img_w, img_h;
};

