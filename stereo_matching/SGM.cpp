#include "SGM.h"


SGM::SGM(Mat &ll, Mat &rr) : Solver(ll, rr)
{
	cost = new float[img_h * img_w * MAX_DISP];
	sum_of_cost = new float[img_h * img_w * MAX_DISP];

	L1 = new float[img_h * img_w * MAX_DISP];
	L2 = new float[img_h * img_w * MAX_DISP];
	L3 = new float[img_h * img_w * MAX_DISP];
	L4 = new float[img_h * img_w * MAX_DISP];
	min_L1 = new float[img_h * img_w];
	min_L2 = new float[img_h * img_w];
	min_L3 = new float[img_h * img_w];
	min_L4 = new float[img_h * img_w];
	if (USE_8_PATH)
	{
		L5 = new float[img_h * img_w * MAX_DISP];
		L6 = new float[img_h * img_w * MAX_DISP];
		L7 = new float[img_h * img_w * MAX_DISP];
		L8 = new float[img_h * img_w * MAX_DISP];
		min_L5 = new float[img_h * img_w];
		min_L6 = new float[img_h * img_w];
		min_L7 = new float[img_h * img_w];
		min_L8 = new float[img_h * img_w];
	}

	P1 = 5;
	P2 = 40;
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

	// build L1: left -> right
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = 0; j < img_w; j++)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (j == 0)		//init
				{				
					L1[index] = cost[index];
				}
				else
				{
					uint32_t index_L1_prev = i * img_w * MAX_DISP + (j - 1) * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L1[index] = MIN(cost[index_L1_prev + d], cost[index_L1_prev + d_sub_1] + P1);
					L1[index] = MIN(L1[index], cost[index_L1_prev + d_plus_1] + P1);
					L1[index] = MIN(L1[index], min_L1[i * img_w + j - 1] + P2);
					L1[index] += (cost[index] - min_L1[i * img_w + j - 1]);
				}				
			}
			
			// update minL1
			float minL1 = 65535;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L1[index] < minL1)
				{
					minL1 = L1[index];
				}
			}
			min_L1[i * img_w + j] = minL1;
		}
	}

	// build L2: right -> left
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = img_w - 1; j != 65535; j--)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (j == img_w - 1)		//init
				{
					L2[index] = cost[index];
				}
				else
				{
					uint32_t index_L2_prev = i * img_w * MAX_DISP + (j + 1) * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L2[index] = MIN(cost[index_L2_prev + d], cost[index_L2_prev + d_sub_1] + P1);
					L2[index] = MIN(L2[index], cost[index_L2_prev + d_plus_1] + P1);
					L2[index] = MIN(L2[index], min_L2[i * img_w + j + 1] + P2);
					L2[index] += (cost[index] - min_L2[i * img_w + j +1]);
				}
			}

			// update minL2
			float minL2 = 65535;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L2[index] < minL2)
				{
					minL2 = L2[index];
				}
			}
			min_L2[i * img_w + j] = minL2;
		}
	}

	// build L3: top -> down
	for (uint16_t i = 0; i < img_h; i++)
	{
		for (uint16_t j = 0; j < img_w; j++)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (i == 0)		//init
				{
					L3[index] = cost[index];
				}
				else
				{
					uint32_t index_L3_prev = (i - 1) * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L3[index] = MIN(cost[index_L3_prev + d], cost[index_L3_prev + d_sub_1] + P1);
					L3[index] = MIN(L3[index], cost[index_L3_prev + d_plus_1] + P1);
					L3[index] = MIN(L3[index], min_L3[(i - 1) * img_w + j ] + P2);
					L3[index] += (cost[index] - min_L3[(i - 1) * img_w + j]);
				}
			}

			// update minL3
			float minL3 = 65535;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L3[index] < minL3)
				{
					minL3 = L3[index];
				}
			}
			min_L3[i * img_w + j] = minL3;
		}
	}

	// build L4: down -> top
	for (uint16_t i = img_h - 1; i != 65535; i--)
	{
		for (uint16_t j = 0; j < img_w; j++)
		{
			// DP
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (i == img_h - 1)		//init
				{
					L4[index] = cost[index];
				}
				else
				{
					uint32_t index_L4_prev = (i + 1) * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L4[index] = MIN(cost[index_L4_prev + d], cost[index_L4_prev + d_sub_1] + P1);
					L4[index] = MIN(L4[index], cost[index_L4_prev + d_plus_1] + P1);
					L4[index] = MIN(L4[index], min_L4[(i + 1) * img_w + j] + P2);
					L4[index] += (cost[index] - min_L4[(i + 1) * img_w + j]);
				}
			}

			// update minL4
			float minL4 = 65535;
			for (uchar d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (L4[index] < minL4)
				{
					minL4 = L4[index];
				}
			}
			min_L4[i * img_w + j] = minL4;
		}
	}

	if (USE_8_PATH)
	{
		// build L5: lefttop -> rightdown
		for (uint16_t i = 0; i < img_h; i++)
		{
			for (uint16_t j = 0; j < img_w; j++)
			{
				// DP
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i == 0 || j == 0)		//init
					{
						L5[index] = cost[index];
					}
					else
					{
						uint32_t index_L5_prev = (i - 1) * img_w * MAX_DISP + (j - 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L5[index] = MIN(cost[index_L5_prev + d], cost[index_L5_prev + d_sub_1] + P1);
						L5[index] = MIN(L5[index], cost[index_L5_prev + d_plus_1] + P1);
						L5[index] = MIN(L5[index], min_L5[(i - 1) * img_w + j - 1] + P2);
						L5[index] += (cost[index] - min_L5[(i - 1) * img_w + j - 1]);
					}
				}

				// update minL5
				float minL5 = 65535;
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (L5[index] < minL5)
					{
						minL5 = L5[index];
					}
				}
				min_L5[i * img_w + j] = minL5;
			}
		}

		// build L6: righttop -> leftdown
		for (uint16_t i = 0; i < img_h; i++)
		{
			for (uint16_t j = img_w - 1; j != 65535; j--)
			{
				// DP
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i== 0 || j == img_w - 1)		//init
					{
						L6[index] = cost[index];
					}
					else
					{
						uint32_t index_L6_prev = (i - 1) * img_w * MAX_DISP + (j + 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L6[index] = MIN(cost[index_L6_prev + d], cost[index_L6_prev + d_sub_1] + P1);
						L6[index] = MIN(L6[index], cost[index_L6_prev + d_plus_1] + P1);
						L6[index] = MIN(L6[index], min_L6[(i - 1) * img_w + j + 1] + P2);
						L6[index] += (cost[index] - min_L6[(i - 1) * img_w + j + 1]);
					}
				}

				// update minL6
				float minL6 = 65535;
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (L6[index] < minL6)
					{
						minL6 = L6[index];
					}
				}
				min_L6[i * img_w + j] = minL6;
			}
		}

		// build L7: leftdown -> righttop
		for (uint16_t i = img_h - 1; i != 65535; i--)
		{
			for (uint16_t j = 0; j < img_w; j++)
			{
				// DP
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i == img_h - 1 || j == 0)		//init
					{
						L7[index] = cost[index];
					}
					else
					{
						uint32_t index_L7_prev = (i + 1) * img_w * MAX_DISP + (j - 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L7[index] = MIN(cost[index_L7_prev + d], cost[index_L7_prev + d_sub_1] + P1);
						L7[index] = MIN(L7[index], cost[index_L7_prev + d_plus_1] + P1);
						L7[index] = MIN(L7[index], min_L7[(i + 1) * img_w + j - 1] + P2);
						L7[index] += (cost[index] - min_L7[(i + 1) * img_w + j - 1]);
					}
				}

				// update minL7
				float minL7 = 65535;
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (L7[index] < minL7)
					{
						minL7 = L7[index];
					}
				}
				min_L7[i * img_w + j] = minL7;
			}
		}

		// build L8
		for (uint16_t i = img_h - 1; i != 65535; i--)
		{
			for (uint16_t j = img_w - 1; j != 65535; j--)
			{
				// DP
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i == img_h - 1 || j == img_w - 1)		//init
					{
						L8[index] = cost[index];
					}
					else
					{
						uint32_t index_L8_prev = (i + 1) * img_w * MAX_DISP + (j + 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L8[index] = MIN(cost[index_L8_prev + d], cost[index_L8_prev + d_sub_1] + P1);
						L8[index] = MIN(L8[index], cost[index_L8_prev + d_plus_1] + P1);
						L8[index] = MIN(L8[index], min_L8[(i + 1) * img_w + j + 1] + P2);
						L8[index] += (cost[index] - min_L8[(i + 1) * img_w + j + 1]);
					}
				}

				// update minL8
				float minL8 = 65535;
				for (uchar d = 0; d < MAX_DISP; d++)
				{
					uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (L8[index] < minL8)
					{
						minL8 = L8[index];
					}
				}
				min_L8[i * img_w + j] = minL8;
			}
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
				if (USE_8_PATH)
				{
					sum_of_cost[index] += L5[index] + L6[index] + L7[index] + L8[index];
				}
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