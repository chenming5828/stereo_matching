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
}


void Solver::Show_disp()
{
	for (int i = 0; i < img_h; i++)
	{
		uchar* ptr = disp.ptr<uchar>(i);
		for (int j = 0; j < img_w; j++)
		{
			ptr[j] *= 255 / (MAX_DISP - 1);		// for better observation
		}
	}

	Mat debug_view, tmp;
	debug_view = debug_view.zeros(img_h * 2, img_w, CV_8UC1);
	tmp = debug_view(Rect(0, 0, img_w, img_h));
	ll.copyTo(tmp);
	tmp = debug_view(Rect(0, img_h - 1, img_w, img_h));
	disp.copyTo(tmp);

	namedWindow("disp_map", 0);
	imshow("disp_map", debug_view);
	//imwrite("example/result_sgm_sad.jpeg", debug_view);
	imwrite("example/test.jpeg", debug_view);

	waitKey();
	destroyWindow("disp_map");
}


void Solver::Process()
{
	std::cout << "Class Solver does nothing!" << std::endl;
}