#include "Solver.h"


Solver::Solver()
{}


Solver::Solver(Mat &ll, Mat &rr)
{
	this->ll = ll.clone();
	this->rr = rr.clone();
	img_w = ll.cols;
	img_h = rr.rows;
	disp.create(img_h, img_w, CV_8UC1);
	colored_disp.create(img_h, img_w, CV_8UC3);

	// dsi
	cost = new float[img_h * img_w * MAX_DISP];
	cost_table_l = new uint64_t[img_h * img_w];
	cost_table_r = new uint64_t[img_h * img_w];

	if (WEIGHTED_COST)
	{
		weight = new float[WIN_H * WIN_W];
#pragma omp parallel for
		for (int i = 0; i < WIN_H; i++)
		{
			for (int j = 0; j < WIN_W; j++)
			{
				weight[i*WIN_W + j] = exp(((i - WIN_H / 2)*(i - WIN_H / 2) + (j - WIN_W / 2)*(j - WIN_W / 2)) / -25.0);
				//std::cout << weight[i*WIN_W + j] << "\t";
			}
			//std::cout << std::endl;
		}
	}
	//std::cin.get();
}


void Solver::Show_disp()
{
	// left border invalid
	for (int i = 0; i < disp.rows; i++)
	{
		uchar *ptr = disp.ptr<uchar>(i);
		for(int j = 0; j < MAX_DISP; j++)
		{
			ptr[j] = INVALID_DISP;
		}
	}
	imwrite("example/disp_grey.png", disp);
	
	// convert to RGB for better observation
	Colormap();	

	Mat debug_view, tmp;

	debug_view = debug_view.zeros(img_h * 2, img_w, CV_8UC3);
	tmp = debug_view(Rect(0, 0, img_w, img_h));
	cvtColor(ll, ll, CV_GRAY2BGR);
	ll.copyTo(tmp);
	tmp = debug_view(Rect(0, img_h - 1, img_w, img_h));
	colored_disp.copyTo(tmp);

	namedWindow("disp_map", 1);
	imshow("disp_map", debug_view);
	imwrite("example/disp_rgb.png", debug_view);

	waitKey();
	destroyWindow("disp_map");
}


void Solver::Process()
{
	std::cout << "Class Solver does nothing!" << std::endl;
}


void Solver::Build_dsi()
{
#pragma omp parallel for
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				//cost[index] = SSD(ll, rr, Point(j, i), d, WIN_H, WIN_W, weight);
				cost[index] = CT(ll, rr, Point(j, i), d, WIN_H, WIN_W, weight);

				//std::cout << "[" << i << ", " << j << ", " << (int)d << "]:\t" <<  cost[index];
				//std::cin.get();
			}
		}
	}
}


void Solver::Build_cost_table()
{
#pragma omp parallel for
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			cost_table_l[i*img_w + j] = CT_pts(ll, j, i, WIN_H, WIN_W, weight);
			cost_table_r[i*img_w + j] = CT_pts(rr, j, i, WIN_H, WIN_W, weight);
		}
	}
}


void Solver::Build_dsi_from_table()
{
#pragma omp parallel for
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				uint64_t ct_l = cost_table_l[i*img_w + j];
				uint64_t ct_r = cost_table_r[i*img_w + MAX(j - d, 0)];
				cost[index] = hamming_cost(ct_l, ct_r);
			}
		}
	}
}


float Solver::Find_dsi_mean_max()
{
	double mean_cost = 0, max_cost = 0;
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			for (int d = 0; d < MAX_DISP; d++)
			{
				int index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				mean_cost += cost[index];
				if (cost[index] > max_cost)
				{
					max_cost = cost[index];
				}
			}
		}
	}
	mean_cost /= (img_h * img_w * MAX_DISP);
	std::cout << "max_cost: " << max_cost << ", mean_cost: " << mean_cost << std::endl;
	return mean_cost;
}


void Solver::cost_horizontal_filter(int win_size)
{
	for (int i = 0; i < img_h; i++)
	{
		for (int d = 0; d < MAX_DISP; d++)
		{
			float sum = 0;
			int index = i * img_w * MAX_DISP + d;
			// initialize
			for (int k = 0; k < win_size; k++)
			{
				sum += cost[index];
				index += MAX_DISP;
			}
			// box filter
			for (int j = win_size/2; j < img_w - win_size/2; j++)
			{
				cost[i * img_w * MAX_DISP + j * MAX_DISP + d] = sum / win_size;
				if (j < img_w - win_size / 2 - 1)
				{
					sum += cost[index];
					sum -= cost[index - win_size * MAX_DISP];
					index += MAX_DISP;
				}
			}
		}
	}
}


void Solver::cost_vertical_filter(int win_size)
{
	for (int j = 0; j < img_w; j++)
	{
		for (int d = 0; d < MAX_DISP; d++)
		{
			float sum = 0;
			int index = j * MAX_DISP + d;
			// initialize
			for (int k = 0; k < win_size; k++)
			{
				sum += cost[index];
				index += img_w * MAX_DISP;
			}
			// box filter
			for (int i = win_size/2; i < img_h - win_size/2; i++)
			{
				cost[i * img_w * MAX_DISP + j * MAX_DISP + d] = sum / win_size;
				if (i < img_h - win_size / 2 - 1)
				{
					sum += cost[index];
					sum -= cost[index - win_size * img_w * MAX_DISP];
					index += img_w * MAX_DISP;
				}
			}
		}
	}
}


void  Solver::Colormap()
{
	int disp_value = 0;
	for (int i = 0; i < disp.rows; i++)
	{
		for (int j = 0; j < disp.cols; j++)
		{
			disp_value = disp.at<uchar>(i, j);
			if (disp_value == INVALID_DISP)
			{
				colored_disp.at<Vec3b>(i, j)[0] = 0;
				colored_disp.at<Vec3b>(i, j)[1] = 0;
				colored_disp.at<Vec3b>(i, j)[2] = 0;
			}
			else
			{
				disp_value *= (256 / (MAX_DISP));
				if (disp_value <= 51)
				{
					colored_disp.at<Vec3b>(i, j)[0] = 255;
					colored_disp.at<Vec3b>(i, j)[1] = disp_value * 5;
					colored_disp.at<Vec3b>(i, j)[2] = 0;
				}
				else if (disp_value <= 102)
				{
					disp_value -= 51;
					colored_disp.at<Vec3b>(i, j)[0] = 255 - disp_value * 5;
					colored_disp.at<Vec3b>(i, j)[1] = 255;
					colored_disp.at<Vec3b>(i, j)[2] = 0;
				}
				else if (disp_value <= 153)
				{
					disp_value -= 102;
					colored_disp.at<Vec3b>(i, j)[0] = 0;
					colored_disp.at<Vec3b>(i, j)[1] = 255;
					colored_disp.at<Vec3b>(i, j)[2] = disp_value * 5;
				}
				else if (disp_value <= 204)
				{
					disp_value -= 153;
					colored_disp.at<Vec3b>(i, j)[0] = 0;
					colored_disp.at<Vec3b>(i, j)[1] = 255 - uchar(128.0*disp_value / 51.0 + 0.5);
					colored_disp.at<Vec3b>(i, j)[2] = 255;
				}
				else
				{
					disp_value -= 204;
					colored_disp.at<Vec3b>(i, j)[0] = 0;
					colored_disp.at<Vec3b>(i, j)[1] = 127 - uchar(127.0*disp_value / 51.0 + 0.5);
					colored_disp.at<Vec3b>(i, j)[2] = 255;
				}
			}
		}
	}
}


Solver::~Solver()
{
	delete[] cost;
	if (WEIGHTED_COST)
	{
		delete[] weight;
	}
}