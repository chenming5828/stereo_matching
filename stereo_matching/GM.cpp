#include "GM.h"


GM::GM(Mat &ll, Mat &rr) : Solver(ll, rr)
{}


void GM::Process()
{
	// build dsi
	Build_dsi();

	uchar *ptr = NULL;
	float min_cost = FLT_MAX, sec_min_cost = FLT_MAX;
	uchar min_d = INVALID_DISP, sec_min_d = INVALID_DISP;
	for (int i = 0; i < img_h; i++)
	{
		ptr = disp.ptr<uchar>(i);
		for (int j = 0; j < img_w; j++)
		{
			min_cost = FLT_MAX;
			min_d = INVALID_DISP;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (cost[index] < min_cost)
				{
					min_cost = cost[index];
					min_d = d;
				}
			}
			// unique check
			sec_min_cost = FLT_MAX;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (cost[index] < sec_min_cost && cost[index] != min_cost)
				{
					sec_min_cost = cost[index];
					sec_min_d = d;
				}
			}
			if (min_cost / sec_min_cost > UNIQUE_RATIO && abs(min_d - sec_min_d) > 2)
			{
				ptr[j] = INVALID_DISP;
			}
			else
			{
				ptr[j] = min_d;
			}
		}
	}
	ptr = NULL;
	//// speckle_filter
	//uint16_t maxsize = 750, maxdiff = 2;
	//filterSpeckles(disp, INVALID_DISP, maxsize, maxdiff);
}


GM::~GM()
{}