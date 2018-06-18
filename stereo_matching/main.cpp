#include <iostream>
#include <core/core.hpp>
#include <highgui/highgui.hpp>

#include "Solver.h"
#include "GM.h"

using namespace cv;

int main()
{
	Mat ll = imread("example/left.jpeg");
	Mat rr = imread("example/right.jpeg");
	//namedWindow("left", 1);
	//imshow("left", ll);
	//namedWindow("right", 1);
	//imshow("right", rr);
	//waitKey();

	Solver *sv = new GM(ll.rows, ll.cols);

	std::cout << "waiting ... " << std::endl;
	sv->Process(ll, rr);
	std::cout << "done ... " << std::endl;
	sv->Show_disp();

	std::cin.get();
	return 0;
}