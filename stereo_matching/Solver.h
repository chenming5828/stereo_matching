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
const bool WEIGHTED_COST = 0;
const int MEDIAN_FILTER_H = 5;
const int MEDIAN_FILTER_W = 5;
const int SPECKLE_SIZE = 1000;
const int SPECKLE_DIS = 2;


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
	float Find_table_mean_max();
	void cost_horizontal_filter(int win_size);
	void cost_vertical_filter(int win_size);
	void fetch_cost(float *p);
	void post_filter();
	void Colormap();
	Mat get_disp() const
	{
		return filtered_disp;
	}
	virtual ~Solver();

protected:
	Mat ll, rr;
	Mat disp,  filtered_disp, colored_disp;
	int img_w, img_h;
	uint64_t *cost_table_l, *cost_table_r;
	float *cost;
	float *weight;
};

