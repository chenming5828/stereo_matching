#include "../global.h"
#include "../gpu_inc/cost.cuh"


const int IMG_W = 1240;
const int IMG_H = 360;

const int CU_MAX_DISP = 128;
const int CU_INVALID_DISP = CU_MAX_DISP + 1;
const int CU_WIN_H = 7;
const int CU_WIN_W = 9;
const int CU_COST_WIN_H = 5;
const int CU_COST_WIN_W = 7;
const float CU_UNIQUE_RATIO = 0.8;
const bool CU_WEIGHTED_COST = 1;
const int CU_MEDIAN_FILTER_H = 5;
const int CU_MEDIAN_FILTER_W = 5;
const int CU_SPECKLE_SIZE = 1000;
const int CU_SPECKLE_DIS = 2;

const bool CU_USE_8_PATH = 1;


class GPU_SGM
{
public:
	GPU_SGM();
	~GPU_SGM();
	void Process(Mat &ll, Mat &rr, Mat &disp, float *cost);

private:
	uchar *d_ll, *d_rr;
	uchar *d_disp, *d_colored_disp;
	float *d_filtered_disp;
	uint64_t *d_cost_table_l, *d_cost_table_r;
	float *d_cost;
	float *d_weight;

	float *d_L1, *d_L2, *d_L3, *d_L4, *d_L5, *d_L6, *d_L7, *d_L8;
	float *d_min_L1, *d_min_L2, *d_min_L3, *d_min_L4, *d_min_L5, *d_min_L6, *d_min_L7, *d_min_L8;
	int P1, P2;
};
