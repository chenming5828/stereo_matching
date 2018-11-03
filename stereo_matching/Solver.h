#pragma once

#include "global.h"
#include "cost.h"


const int MAX_DISP = 128;
const int INVALID_DISP = MAX_DISP + 1;
const int WIN_H = 7;
const int WIN_W = 9;
const int COST_WIN_H = 5;
const int COST_WIN_W = 7;
const float UNIQUE_RATIO = 0.8;
const bool WEIGHTED_COST = 1;


class Solver
{
public:
	Solver();
	Solver(Mat &ll, Mat &rr);
	void Show_disp(); 
	virtual void Process();
	void Build_dsi();
	void Build_cost_table();
	void Build_dsi_from_table();
	float Find_dsi_mean_max();
	void cost_horizontal_filter(int win_size);
	void cost_vertical_filter(int win_size);
	void Colormap();
	Mat get_disp() const
	{
		return disp;
	}
	virtual ~Solver();

protected:
	Mat ll, rr;
	Mat disp,  colored_disp;
	int img_w, img_h;
	uint64_t *cost_table_l, *cost_table_r;
	float *cost;
	float *weight;
};

