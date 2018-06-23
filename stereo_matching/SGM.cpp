#include "SGM.h"


SGM::SGM(Mat &ll, Mat &rr) : Solver(ll, rr)
{
	cost = new float[img_h * img_w * MAX_DISP];
	L1 = new float[img_h * img_w * MAX_DISP];
	sum_of_cost = new float[img_h * img_w * MAX_DISP];
	P1 = 10;
	P2 = 50;
}


void SGM::Process()
{
	// build dsi
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = 0; j < img_w; j++)
		{
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				cost[index] = SAD(ll, rr, Point(j, i), d, WIN_H, WIN_W);
				//std::cout << cost[index] << '\t';
			}
		}
	}

	// build L1
	float minL1 = 32767;
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = 0; j < img_w; j++)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				if (!j)	//init
				{
					uint32_t index = i * img_w * MAX_DISP  + d;
					L1[index] = cost[index];
				}
				else
				{
					uint32_t index_L1_prev = i * img_w * MAX_DISP + (j - 1) * MAX_DISP;
					uint32_t index_L1_cur = i * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L1[index_L1_cur + d] = MIN(cost[index_L1_prev + d], cost[index_L1_prev + d_sub_1] + P1);
					L1[index_L1_cur + d] = MIN(L1[index_L1_cur + d], cost[index_L1_prev + d_plus_1] + P1);
					L1[index_L1_cur + d] = MIN(L1[index_L1_cur + d], minL1 + P2);
					L1[index_L1_cur + d] += cost[index_L1_cur + d];
				}				
			}
			
			// update minL1
			minL1 = 32767;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L1[index] < minL1)
				{
					minL1 = L1[index];
				}
			}
		}
	}

	// cost aggregation
	uchar *ptr = NULL;
	float min_cost = 32767;
	uchar min_d = INVALID_DISP;
	for (uint16_t i = 0; i < img_h; i++)
	{
		ptr = disp.ptr<uchar>(i);
		for (uint16_t j = 0; j < img_w; j++)
		{
			min_cost = 32767;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				sum_of_cost[index] = L1[index];
				if (sum_of_cost[index] < min_cost)
				{
					min_cost = sum_of_cost[index];
					min_d = d;
				}
			}
			ptr[j] = min_d;
		}
	}
	ptr = NULL;
}


SGM::~SGM()
{
	delete[] cost;
	delete[] L1;
	delete[] sum_of_cost;
}