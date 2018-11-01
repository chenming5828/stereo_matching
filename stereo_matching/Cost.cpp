#include "cost.h"


float SAD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w, float* weight)
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
			x_r = MAX(x_l - disp, 0);
			if (WEIGHTED_COST)
			{
				cost += abs(ll_ptr[x_l] - rr_ptr[x_r]) * weight[(i + win_h / 2) * win_w + (j + win_w / 2)];
			}
			else
			{
				cost += abs(ll_ptr[x_l] - rr_ptr[x_r]);
			}
		}
	}
	ll_ptr = rr_ptr = NULL;
	return cost / win_h / win_w;
}


float SSD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w, float* weight)
{
	uchar *ll_ptr = NULL, *rr_ptr = NULL;
	uint16_t y = 0, x_l = 0, x_r = 0;
	float cost = 0;
	for (char i = -win_h / 2; i <= win_h / 2; i++)
	{
		y = MAX(l_pt.y + i, 0);		// check border
		y = MIN(y, ll.rows - 1);
		ll_ptr = ll.ptr<uchar>(y);
		rr_ptr = rr.ptr<uchar>(y);
		for (char j = -win_w / 2; j <= win_w / 2; j++)
		{
			x_l = MAX(l_pt.x + j, 0);
			x_l = MIN(x_l, ll.cols - 1);
			x_r = MAX(x_l - disp, 0);
			if (WEIGHTED_COST)
			{
				cost += (ll_ptr[x_l] - rr_ptr[x_r]) * (ll_ptr[x_l] - rr_ptr[x_r]) * weight[(i + win_h / 2) * win_w + (j + win_w / 2)];
			}
			else
			{
				cost += (ll_ptr[x_l] - rr_ptr[x_r]) * (ll_ptr[x_l] - rr_ptr[x_r]);
			}
		}
	}
	ll_ptr = rr_ptr = NULL;
	return cost / win_h / win_w;		// be careful of overflow
}


uint16_t CT(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w, float* weight)
{
	uchar *ll_ptr = NULL, *rr_ptr = NULL;
	uint16_t y = 0, x_l = 0, x_r = 0;
	uint64_t ct_l = 0, ct_r = 0;
	uint16_t cost = 0;

	uchar ctr_pixel_l = ll.at<uchar>(l_pt.y, l_pt.x);
	uchar ctr_pixel_r = rr.at<uchar>(l_pt.y, MAX(l_pt.x - disp, 0));

	for (char i = -win_h / 2; i <= win_h / 2; i++)
	{
		y = MAX(l_pt.y + i, 0);		// check border
		y = MIN(y, ll.rows - 1);
		ll_ptr = ll.ptr<uchar>(y);
		rr_ptr = rr.ptr<uchar>(y);
		for (char j = -win_w / 2; j <= win_w / 2; j++)
		{
			if (i == 0 && j == 0)
				continue;
			if (WEIGHTED_COST && weight[(i + win_h / 2) * win_w + (j + win_w / 2)] < 0.5)
				continue;
			x_l = MAX(l_pt.x + j, 0);
			x_l = MIN(x_l, ll.cols - 1);
			x_r = MAX(x_l - disp, 0);
			ct_l = (ct_l | (ll_ptr[x_l] > ctr_pixel_l)) << 1;
			ct_r = (ct_r | (rr_ptr[x_r] > ctr_pixel_r)) << 1;
		}
	}
	cost = hamming_cost(ct_l, ct_r);
	ll_ptr = rr_ptr = NULL;
	return cost;
}


uint64_t CT_pts(Mat &im, int u, int v, uchar win_h, uchar win_w, float* weight)
{
	uchar *ptr = NULL;
	uint16_t y = 0, x = 0;
	uint64_t value = 0;

	uchar ctr_pixel = im.at<uchar>(v,u);

	for (char i = -win_h / 2; i <= win_h / 2; i++)
	{
		y = MAX(v + i, 0);		// check border
		y = MIN(y, im.rows - 1);
		ptr = im.ptr<uchar>(y);
		for (char j = -win_w / 2; j <= win_w / 2; j++)
		{
			if (WEIGHTED_COST && weight[(i + win_h / 2) * win_w + (j + win_w / 2)] < 0.5)
				continue;
			x = MAX(u + j, 0);
			x = MIN(x, im.cols - 1);
			value = (value | (ptr[x] > ctr_pixel)) << 1;
		}
	}
	ptr = NULL;
	return value;
}


uint16_t hamming_cost(uint64_t ct_l, uint64_t ct_r)
{
	uint64_t not_the_same = ct_l ^ ct_r;
	// find the number of '1', log(N)
	uint16_t cnt = 0;
	while (not_the_same)
	{
		//std::cout << not_the_same << std::endl;
		cnt += (not_the_same & 1);
		not_the_same >>= 1;
	}
	return cnt;
}