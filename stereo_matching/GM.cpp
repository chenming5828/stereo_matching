#include "GM.h"


GM::GM(Mat &ll, Mat &rr) : Solver(ll, rr)
{}


void GM::Process()
{
	uchar *ptr = NULL;
	uchar min_d = INVALID_DISP;
	float cost = 0, min_cost = 32767;
	for (uint16_t i = 0; i < img_h; i++)
	{
		ptr = disp.ptr<uchar>(i);
		for (uint16_t j = 0; j < img_w; j++)
		{
			min_cost = 32767;
			min_d = INVALID_DISP;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				cost = SAD(ll, rr, Point(j, i), d, WIN_H, WIN_W);
				if (cost < min_cost)
				{
					min_cost = cost;
					min_d = d;
				}
			}
			ptr[j] = min_d;
			//std::cout << min_cost << " " << (int)min_d << std::endl;
		}
	}
	ptr = NULL;
}