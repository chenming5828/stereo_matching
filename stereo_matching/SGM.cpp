#include "SGM.h"


SGM::SGM(Mat &ll, Mat &rr) : Solver(ll, rr)
{
	cost = new float[img_h * img_w * MAX_DISP];
	L1 = new float[img_h * img_w * MAX_DISP];
	L2 = new float[img_h * img_w * MAX_DISP];
	L3 = new float[img_h * img_w * MAX_DISP];
	L4 = new float[img_h * img_w * MAX_DISP];
	min_L1 = new float[img_h * img_w];
	min_L2 = new float[img_h * img_w];
	min_L3 = new float[img_h * img_w];
	min_L4 = new float[img_h * img_w];
	sum_of_cost = new float[img_h * img_w * MAX_DISP];
	P1 = 5;
	P2 = 30;
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
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = 0; j < img_w; j++)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				if (!j)	//init
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
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
					L1[index_L1_cur + d] = MIN(L1[index_L1_cur + d], min_L1[i * img_w + j - 1] + P2);
					L1[index_L1_cur + d] += (cost[index_L1_cur + d] - min_L1[i * img_w + j - 1]);
				}				
			}
			
			// update minL1
			float minL1 = 65535;
			uint32_t index;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L1[index] < minL1)
				{
					minL1 = L1[index];
				}
			}
			min_L1[i * img_w + j] = minL1;
		}
	}

	// build L2
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = img_w - 1; j != 65535; j--)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				if (j == img_w - 1)	//init
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					L2[index] = cost[index];
				}
				else
				{
					uint32_t index_L2_prev = i * img_w * MAX_DISP + (j + 1) * MAX_DISP;
					uint32_t index_L2_cur = i * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L2[index_L2_cur + d] = MIN(cost[index_L2_prev + d], cost[index_L2_prev + d_sub_1] + P1);
					L2[index_L2_cur + d] = MIN(L2[index_L2_cur + d], cost[index_L2_prev + d_plus_1] + P1);
					L2[index_L2_cur + d] = MIN(L2[index_L2_cur + d], min_L2[i * img_w + j + 1] + P2);
					L2[index_L2_cur + d] += (cost[index_L2_cur + d] - min_L2[i * img_w + j +1]);
				}
			}

			// update minL2
			float minL2 = 65535;
			uint32_t index;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L2[index] < minL2)
				{
					minL2 = L2[index];
				}
			}
			min_L2[i * img_w + j] = minL2;
		}
	}

	// build L3
	for (uint16_t j = 0; j < img_w; j++)
	{
		for (uint16_t i = 0; i < img_h; i++)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				if (!i)	//init
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					L3[index] = cost[index];
				}
				else
				{
					uint32_t index_L3_prev = (i - 1) * img_w * MAX_DISP + j * MAX_DISP;
					uint32_t index_L3_cur = i * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L3[index_L3_cur + d] = MIN(cost[index_L3_prev + d], cost[index_L3_prev + d_sub_1] + P1);
					L3[index_L3_cur + d] = MIN(L3[index_L3_cur + d], cost[index_L3_prev + d_plus_1] + P1);
					L3[index_L3_cur + d] = MIN(L3[index_L3_cur + d], min_L3[(i - 1) * img_w + j ] + P2);
					L3[index_L3_cur + d] += (cost[index_L3_cur + d] - min_L3[(i - 1) * img_w + j]);
				}
			}

			// update minL3
			float minL3 = 65535;
			uint32_t index;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L3[index] < minL3)
				{
					minL3 = L3[index];
				}
			}
			min_L3[i * img_w + j] = minL3;
		}
	}

	// build L4
	for (uint16_t j = 0; j < img_w; j++)
	{
		for (uint16_t i = img_h - 1; i != 65535; i--)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				if (i == img_h - 1)	//init
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					L4[index] = cost[index];
				}
				else
				{
					uint32_t index_L4_prev = (i + 1) * img_w * MAX_DISP + j * MAX_DISP;
					uint32_t index_L4_cur = i * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L4[index_L4_cur + d] = MIN(cost[index_L4_prev + d], cost[index_L4_prev + d_sub_1] + P1);
					L4[index_L4_cur + d] = MIN(L4[index_L4_cur + d], cost[index_L4_prev + d_plus_1] + P1);
					L4[index_L4_cur + d] = MIN(L4[index_L4_cur + d], min_L4[(i + 1) * img_w + j] + P2);
					L4[index_L4_cur + d] += (cost[index_L4_cur + d] - min_L4[(i + 1) * img_w + j]);
				}
			}

			// update minL4
			float minL4 = 65535;
			uint32_t index;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L4[index] < minL4)
				{
					minL4 = L4[index];
				}
			}
			min_L4[i * img_w + j] = minL4;
		}
	}

	// cost aggregation
	uchar *ptr = NULL;
	float min_cost = 65535;
	uchar min_d = INVALID_DISP;
	for (uint16_t i = 0; i < img_h; i++)
	{
		ptr = disp.ptr<uchar>(i);
		for (uint16_t j = 0; j < img_w; j++)
		{
			min_cost = 65535;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				sum_of_cost[index] = L1[index] + L2[index] + L3[index] + L4[index];
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