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
				cost = SSD(ll, rr, Point(j, i), d, WIN_H, WIN_W);
				// wta
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


//// for  dsi debug
//void GM::Process()
//{
//	float *cost = new float[img_h * img_w * MAX_DISP]; 
//	for (uint16_t i = 0; i < img_h; i++)
//	{
//		for (uint16_t j = 0; j < img_w; j++)
//		{
//			for (uchar d = 0; d < MAX_DISP; d++)
//			{
//				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
//				cost[index] = SSD(ll, rr, Point(j, i), d, WIN_H, WIN_W);
//				std::cout << "[" << i << ", " << j << ", " << (int)d << "]:\t" <<  cost[index];
//				std::cin.get();
//			}
//		}
//	}
//
//	uchar *ptr = NULL;
//	uchar min_d = INVALID_DISP;
//	float min_cost = 32767;
//	for (uint16_t i = 0; i < img_h; i++)
//	{
//		ptr = disp.ptr<uchar>(i);
//		for (uint16_t j = 0; j < img_w; j++)
//		{
//			min_cost = 32767;
//			min_d = INVALID_DISP;
//			for (uchar d = 0; d < MAX_DISP; d++)
//			{
//				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
//				if (cost[index] < min_cost)
//				{
//					min_cost = cost[index];
//					min_d = d;
//				}
//			}
//			ptr[j] = min_d;
//		}
//	}
//	ptr = NULL;
//}