#include "global.h"
#include "GM.h"
#include "SGM.h"
#include "utils.h"


int main()
{
	Mat ll = imread("example/ori_left.jpeg", IMREAD_GRAYSCALE);
	Mat rr = imread("example/ori_right.jpeg", IMREAD_GRAYSCALE);
	std::cout << "rows: " << ll.rows << ", cols: " << ll.cols << std::endl;
	//namedWindow("left", 1);
	//imshow("left", ll);
	//namedWindow("right", 1);
	//imshow("right", rr);
	//waitKey();

	//Solver *sv = new GM(ll, rr);
	Solver *sv = new SGM(ll, rr);

	std::cout << "waiting ... " << std::endl;
	double be = get_cur_ms();
	sv->Process();
	double en = get_cur_ms();
	std::cout << "done ... " << std::endl;
	std::cout << "time cost: " << en - be << " ms" << std::endl;
	sv->Show_disp();

	//std::cin.get();
	return 0;
}