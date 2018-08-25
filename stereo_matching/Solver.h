#pragma once

#include "global.h"
#include "cost.h"


const uchar MAX_DISP = 128;
const uchar INVALID_DISP = MAX_DISP + 1;
const uchar WIN_H = 7;
const uchar WIN_W = 9;
const float UNIQUE_RATIO = 0.9;
const bool WEIGHTED_COST = 1;


class Solver
{
public:
	Solver();
	Solver(Mat &ll, Mat &rr);
	void Show_disp(); 
	virtual void Process();
	void Build_dsi();
	void Find_dsi_mean_max();
	void Colormap();
	Mat get_disp() const
	{
		return disp;
	}
	virtual ~Solver();

protected:
	Mat ll, rr;
	Mat disp,  colored_disp;
	uint16_t img_w, img_h;
	float *cost;
	float *weight;
};

