#include "Cost.h"


float SAD(cv::Mat ll, cv::Mat rr, cv::Point l_pt, unsigned int disp, unsigned win_w, unsigned win_h)
{
	uchar *ll_ptr = NULL, *rr_ptr = NULL;
	unsigned int h = 0, w_l = 0, w_r = 0;
	float cost = 0;
	for (int i = -win_h / 2; i < win_h/2; i++)
	{
		h = MAX(l_pt.y + i, 0);		// check border
		h = MIN(h, ll.rows);
		ll_ptr = ll.ptr<uchar>(h);
		rr_ptr = rr.ptr<uchar>(h);
		for (int j = -win_w / 2; j < win_w / 2; j++)
		{
			w_l = MAX(l_pt.x + j, 0);
			w_l = MIN(w_l, ll.cols);
			w_r = MAX(l_pt.x + j - disp, 0);
			cost += abs(ll_ptr[w_l] - rr_ptr[w_r]);
		}
	}
	ll_ptr = NULL;
	rr_ptr = NULL;
	cost /= (win_w * win_h);
	return cost;
}

