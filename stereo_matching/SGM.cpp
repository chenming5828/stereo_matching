#include "SGM.h"


SGM::SGM(Mat &ll, Mat &rr) : Solver(ll, rr)
{
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

	P1 = 10;
	P2 = 100;
}


void SGM::Process()
{
	// build dsi
	//Build_dsi();
	//Find_dsi_mean_max();

	//Build_cost_table();
	//Build_dsi_from_table();
	//Find_table_mean_max();
	//Find_dsi_mean_max();

	cost_horizontal_filter(COST_WIN_W);
	cost_vertical_filter(COST_WIN_H);

	 //build L1: left -> right
#pragma omp parallel for
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			// DP
			float minL1 = FLT_MAX;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (j == 0)		//init
				{				
					L1[index] = cost[index];
				}
				else
				{
					int index_L1_prev = i * img_w * MAX_DISP + (j - 1) * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L1[index] = MIN(L1[index_L1_prev + d], L1[index_L1_prev + d_sub_1] + P1);
					L1[index] = MIN(L1[index], L1[index_L1_prev + d_plus_1] + P1);
					L1[index] = MIN(L1[index], min_L1[i * img_w + j - 1] + P2);
					L1[index] += (cost[index] - min_L1[i * img_w + j - 1]);
				}
				if (L1[index] < minL1)
				{
					minL1 = L1[index];
				}
			}
			
			// update minL1
			min_L1[i * img_w + j] = minL1;
		}
	}

	// build L2: right -> left
#pragma omp parallel for
	for (int i = 0; i < img_h; i++)
	{
		for (int j = img_w - 1; j >=0; j--)
		{
			// DP
			float minL2 = FLT_MAX;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (j == img_w - 1)		//init
				{
					L2[index] = cost[index];
				}
				else
				{
					int index_L2_prev = i * img_w * MAX_DISP + (j + 1) * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L2[index] = MIN(L2[index_L2_prev + d], L2[index_L2_prev + d_sub_1] + P1);
					L2[index] = MIN(L2[index], L2[index_L2_prev + d_plus_1] + P1);
					L2[index] = MIN(L2[index], min_L2[i * img_w + j + 1] + P2);
					L2[index] += (cost[index] - min_L2[i * img_w + j +1]);
				}
				if (L2[index] < minL2)
				{
					minL2 = L2[index];
				}
			}

			// update minL2
			min_L2[i * img_w + j] = minL2;
		}
	}

	// build L3: top -> down
#pragma omp parallel for
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			// DP
			float minL3 = FLT_MAX;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (i == 0)		//init
				{
					L3[index] = cost[index];
				}
				else
				{
					int index_L3_prev = (i - 1) * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L3[index] = MIN(L3[index_L3_prev + d], L3[index_L3_prev + d_sub_1] + P1);
					L3[index] = MIN(L3[index], L3[index_L3_prev + d_plus_1] + P1);
					L3[index] = MIN(L3[index], min_L3[(i - 1) * img_w + j ] + P2);
					L3[index] += (cost[index] - min_L3[(i - 1) * img_w + j]);
				}
				if (L3[index] < minL3)
				{
					minL3 = L3[index];
				}
			}

			// update minL3
			min_L3[i * img_w + j] = minL3;
		}
	}

	// build L4: down -> top
#pragma omp parallel for
	for (int i = img_h - 1; i >=0; i--)
	{
		for (int j = 0; j < img_w; j++)
		{
			// DP
			float minL4 = FLT_MAX;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				if (i == img_h - 1)		//init
				{
					L4[index] = cost[index];
				}
				else
				{
					int index_L4_prev = (i + 1) * img_w * MAX_DISP + j * MAX_DISP;
					uchar d_sub_1 = MAX(d - 1, 0);
					uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
					L4[index] = MIN(L4[index_L4_prev + d], L4[index_L4_prev + d_sub_1] + P1);
					L4[index] = MIN(L4[index], L4[index_L4_prev + d_plus_1] + P1);
					L4[index] = MIN(L4[index], min_L4[(i + 1) * img_w + j] + P2);
					L4[index] += (cost[index] - min_L4[(i + 1) * img_w + j]);
				}
				if (L4[index] < minL4)
				{
					minL4 = L4[index];
				}
			}

			// update minL4
			min_L4[i * img_w + j] = minL4;
		}
	}

	if (USE_8_PATH)
	{
		// build L5: lefttop -> rightdown
#pragma omp parallel for
		for (int i = 0; i < img_h; i++)
		{
			for (int j = 0; j < img_w; j++)
			{
				// DP
				float minL5 = FLT_MAX;
				for (int d = 0; d < MAX_DISP; d++)
				{
					int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i == 0 || j == 0)		//init
					{
						L5[index] = cost[index];
					}
					else
					{
						int index_L5_prev = (i - 1) * img_w * MAX_DISP + (j - 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L5[index] = MIN(L5[index_L5_prev + d], L5[index_L5_prev + d_sub_1] + P1);
						L5[index] = MIN(L5[index], L5[index_L5_prev + d_plus_1] + P1);
						L5[index] = MIN(L5[index], min_L5[(i - 1) * img_w + j - 1] + P2);
						L5[index] += (cost[index] - min_L5[(i - 1) * img_w + j - 1]);
					}
					if (L5[index] < minL5)
					{
						minL5 = L5[index];
					}
				}

				// update minL5
				min_L5[i * img_w + j] = minL5;
			}
		}

		// build L6: righttop -> leftdown
#pragma omp parallel for
		for (int i = 0; i < img_h; i++)
		{
			for (int j = img_w - 1; j >=0; j--)
			{
				// DP
				float minL6 = FLT_MAX;
				for (int d = 0; d < MAX_DISP; d++)
				{
					int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i== 0 || j == img_w - 1)		//init
					{
						L6[index] = cost[index];
					}
					else
					{
						int index_L6_prev = (i - 1) * img_w * MAX_DISP + (j + 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L6[index] = MIN(L6[index_L6_prev + d], L6[index_L6_prev + d_sub_1] + P1);
						L6[index] = MIN(L6[index], L6[index_L6_prev + d_plus_1] + P1);
						L6[index] = MIN(L6[index], min_L6[(i - 1) * img_w + j + 1] + P2);
						L6[index] += (cost[index] - min_L6[(i - 1) * img_w + j + 1]);
					}
					if (L6[index] < minL6)
					{
						minL6 = L6[index];
					}
				}

				// update minL6
				min_L6[i * img_w + j] = minL6;
			}
		}

		// build L7: leftdown -> righttop
#pragma omp parallel for
		for (int i = img_h - 1; i >=0; i--)
		{
			for (int j = 0; j < img_w; j++)
			{
				// DP
				float minL7 = FLT_MAX;
				for (int d = 0; d < MAX_DISP; d++)
				{
					int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i == img_h - 1 || j == 0)		//init
					{
						L7[index] = cost[index];
					}
					else
					{
						int index_L7_prev = (i + 1) * img_w * MAX_DISP + (j - 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L7[index] = MIN(L7[index_L7_prev + d], L7[index_L7_prev + d_sub_1] + P1);
						L7[index] = MIN(L7[index], L7[index_L7_prev + d_plus_1] + P1);
						L7[index] = MIN(L7[index], min_L7[(i + 1) * img_w + j - 1] + P2);
						L7[index] += (cost[index] - min_L7[(i + 1) * img_w + j - 1]);
					}
					if (L7[index] < minL7)
					{
						minL7 = L7[index];
					}
				}

				// update minL7
				min_L7[i * img_w + j] = minL7;
			}
		}

		// build L8: rightdown -> lefttop
#pragma omp parallel for
		for (int i = img_h - 1; i >=0; i--)
		{
			for (int j = img_w - 1; j >=0; j--)
			{
				// DP
				float minL8 = FLT_MAX;
				for (int d = 0; d < MAX_DISP; d++)
				{
					int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
					if (i == img_h - 1 || j == img_w - 1)		//init
					{
						L8[index] = cost[index];
					}
					else
					{
						int index_L8_prev = (i + 1) * img_w * MAX_DISP + (j + 1) * MAX_DISP;
						uchar d_sub_1 = MAX(d - 1, 0);
						uchar d_plus_1 = MIN(d + 1, MAX_DISP - 1);
						L8[index] = MIN(L8[index_L8_prev + d], L8[index_L8_prev + d_sub_1] + P1);
						L8[index] = MIN(L8[index], L8[index_L8_prev + d_plus_1] + P1);
						L8[index] = MIN(L8[index], min_L8[(i + 1) * img_w + j + 1] + P2);
						L8[index] += (cost[index] - min_L8[(i + 1) * img_w + j + 1]);
					}
					if (L8[index] < minL8)
					{
						minL8 = L8[index];
					}
				}

				// update minL8
				min_L8[i * img_w + j] = minL8;
			}
		}
	}
	
	// cost aggregation
	uchar *ptr = NULL;
	float min_cost = FLT_MAX, sec_min_cost = FLT_MAX;
	uchar min_d = INVALID_DISP, sec_min_d = INVALID_DISP;
	for (int i = 0; i < img_h; i++)
	{
		ptr = disp.ptr<uchar>(i);
		for (int j = 0; j < img_w; j++)
		{
			min_cost = FLT_MAX;
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				cost[index] = 1.2*L1[index] + 1.2*L2[index] + L3[index] + L4[index];  // add horizontal constraint
				//cost[index] = L1[index];
				if (USE_8_PATH)
				{
					cost[index] += (L5[index] + L6[index] + L7[index] + L8[index]);
					//cost[index] = L1[index] + L2[index];
				}
				// wta
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
			if (min_cost / sec_min_cost > UNIQUE_RATIO && abs(min_d - sec_min_d) > 1)
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

	post_filter();
}


SGM::~SGM()
{
	delete[] L1;
	delete[] L2;
	delete[] L3;
	delete[] L4;
	delete[] min_L1;
	delete[] min_L2;
	delete[] min_L3;
	delete[] min_L4;
	if (USE_8_PATH)
	{
		delete[] L5;
		delete[] L6;
		delete[] L7;
		delete[] L8;
		delete[] min_L5;
		delete[] min_L6;
		delete[] min_L7;
		delete[] min_L8;
	}
}