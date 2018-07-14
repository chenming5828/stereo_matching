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
	for (int i = 0; i < img_h; i++)
	{
		uchar* ptr = disp.ptr<uchar>(i);
		for (int j = 0; j < img_w; j++)
			ptr[j] = INVALID_DISP;
	}
	disp_float.create(img_h, img_w, CV_32FC(INVALID_DISP));
	disp_float_colored.create(img_h, img_w, CV_32FC3);

	// dsi
	cost = new float[img_h * img_w * MAX_DISP];
	memset(cost, 65536, sizeof(cost));
}


void Solver::Show_disp()
{
	for (int i = 0; i < img_h; i++)
	{
		uchar* ptr = disp.ptr<uchar>(i);
		for (int j = 0; j < img_w; j++)
		{
			ptr[j] *= (256 / (MAX_DISP));		// for better observation
		}
	}

	Mat debug_view, tmp;
	debug_view = debug_view.zeros(img_h * 2, img_w, CV_8UC1);
	tmp = debug_view(Rect(0, 0, img_w, img_h));
	ll.copyTo(tmp);
	tmp = debug_view(Rect(0, img_h - 1, img_w, img_h));
	disp.copyTo(tmp);

	namedWindow("disp_map", 1);
	imshow("disp_map", debug_view);
	//imwrite("example/result_sgm.jpeg", debug_view);
	//imwrite("example/result_sgm.png", debug_view);
	//imwrite("example/uni_sgm.png", debug_view);
	imwrite("example/test.png", debug_view);

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
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
				//cost[index] = SSD(ll, rr, Point(j, i), d, WIN_H, WIN_W);
				cost[index] = CT(ll, rr, Point(j, i), d, WIN_H, WIN_W);

				//std::cout << "[" << i << ", " << j << ", " << (int)d << "]:\t" <<  cost[index];
				//std::cin.get();
			}
		}
	}
}


void Solver::Find_dsi_mean_max()
{
	double mean_cost = 0, max_cost = 0;
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			for (int d = 0; d < MAX_DISP; d++)
			{
				uint32_t index = i * img_w * MAX_DISP + j * MAX_DISP + d;
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
	std::cin.get();

}


Solver::~Solver()
{
	delete[] cost;
}