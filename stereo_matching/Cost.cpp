#include "cost.h"


float SAD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w)
{
	uchar *ll_ptr = NULL, *rr_ptr = NULL;
	uint16_t y = 0, x_l = 0, x_r = 0;
	float cost = 0;
	for (char i = -win_h / 2; i <= win_h/2; i++)
	{
		y = MAX(l_pt.y + i, 0);		// check border
		y = MIN(y, ll.rows - 1);
		ll_ptr = ll.ptr<uchar>(y);
		rr_ptr = rr.ptr<uchar>(y); 
		for (char j = -win_w / 2; j <= win_w / 2; j++)
		{
			x_l = MAX(l_pt.x + j, 0);
			x_l = MIN(x_l, ll.cols - 1);
			x_r = MAX(l_pt.x + j - disp, 0);
			cost += abs(ll_ptr[x_l] - rr_ptr[x_r]);
		}
	}
	ll_ptr = rr_ptr = NULL;
	cost /= (win_w * win_h);
	return cost;
}


float SSD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w)
{
	uchar *ll_ptr = NULL, *rr_ptr = NULL;
	uint16_t y = 0, x_l = 0, x_r = 0;
	float cost = 0;
	for (uchar i = -win_h / 2; i <= win_h / 2; i++)
	{
		y = MAX(l_pt.y + i, 0);		// check border
		y = MIN(y, ll.rows - 1);
		ll_ptr = ll.ptr<uchar>(y);
		rr_ptr = rr.ptr<uchar>(y);
		for (uchar j = -win_w / 2; j <= win_w / 2; j++)
		{
			x_l = MAX(l_pt.x + j, 0);
			x_l = MIN(x_l, ll.cols);
			x_r = MAX(l_pt.x + j - disp, 0);
			cost += (ll_ptr[x_l] - rr_ptr[x_r]) * (ll_ptr[x_l] - rr_ptr[x_r]);
		}
	}
	ll_ptr = rr_ptr = NULL;
	cost /= (win_w * win_h);
	return cost;
}

