#include "GM.h"


GM::GM(unsigned int img_w, unsigned int img_h)
{
	disp.create(img_w, img_h, CV_8UC1);
	for (int i = 0; i < disp.rows; i++)
	{
		uchar* ptr = disp.ptr<uchar>(i);
		for (int j = 0; j < disp.cols; j++)
			ptr[j] = MAX_DISP;
	}
	//for (int i = 0; i < disp.rows; i++)
	//{
	//	uchar* ptr = disp.ptr<uchar>(i);
	//	for (int j = 0; j < disp.cols; j++)
	//		std::cout << (int)ptr[j];
	//}
	disp_float.create(Size(img_w, img_h), CV_32FC(MAX_DISP));
	disp_float_colored.create(Size(img_w, img_h), CV_32FC3);
}


void GM::Process(Mat &ll, Mat &rr)
{
	uchar *ptr = NULL;
	unsigned int h = ll.rows, w = ll.cols, min_d = MAX_DISP;
	float cost = 0, min_cost = 32767;
	for (unsigned int i = 0; i < h; i++)
	{
		ptr = disp.ptr<uchar>(i);
		for (unsigned int j = 0; j < w; j++)
		{
			min_cost = 32767;
			min_d = MAX_DISP;
			for (int d = 0; d < MAX_DISP; d++)
			{
				cost = SAD(ll, rr, Point(j, i), d, 3, 3);
				if (cost < min_cost)
				{
					min_cost = cost;
					min_d = d;
				}
			}
			ptr[j] = min_d;
		}
	}
	ptr = NULL;
}


GM::~GM()
{}