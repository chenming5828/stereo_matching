# include <iostream>
# include <core/core.hpp>
# include <highgui/highgui.hpp>

using namespace cv;

int main()
{
	Mat ll = imread("example/left.jpeg");
	Mat rr = imread("example/right.jpeg");
	namedWindow("left", 1);
	imshow("left", ll);
	namedWindow("right", 1);
	imshow("right", rr);
	waitKey();

	std::cin.get();
	return 0;
}