#include "Solver.h"

Solver::Solver()
{}


void Solver::Show_disp()
{
	namedWindow("disp_map");
	imshow("disp_map", disp);
	//for (int i = 0; i<disp.rows; i++)
	//{
	//	uchar* ptr = disp.ptr<uchar>(i);
	//	for (int j = 0; j<disp.cols; j++)
	//		std::cout << (int)ptr[j];
	//	std::cout  << std::endl;
	//}
	waitKey();
}


void Solver::Process(Mat &ll, Mat &rr)
{}

Solver::~Solver()
{}