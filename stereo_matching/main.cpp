#include "global.h"
#include "GM.h"
#include "SGM.h"
#include "utils.h"


int main()
{
	Mat ll = imread("example/ori_left.png", IMREAD_GRAYSCALE);
	Mat rr = imread("example/ori_right.png", IMREAD_GRAYSCALE);
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

	delete sv;

	// opencv examples
	//Mat disp, disp8;

	//StereoBM bm;
	//bm.state->preFilterCap = 31;
	//bm.state->SADWindowSize = 9;
	//bm.state->minDisparity = 0;
	//bm.state->numberOfDisparities = 64;
	//bm.state->textureThreshold = 10;
	//bm.state->uniquenessRatio = 15;
	//bm.state->speckleWindowSize = 100;
	//bm.state->speckleRange = 32;
	//bm.state->disp12MaxDiff = 1;
	//bm(ll, rr, disp);

	//StereoSGBM sgbm;
	//sgbm.preFilterCap = 63;
	//sgbm.SADWindowSize = 9;
	//sgbm.P1 = 8 *sgbm.SADWindowSize*sgbm.SADWindowSize;
	//sgbm.P2 = 32*sgbm.SADWindowSize*sgbm.SADWindowSize;
	//sgbm.minDisparity = 0;
	//sgbm.numberOfDisparities = 64;
	//sgbm.uniquenessRatio = 10;
	//sgbm.speckleWindowSize = 100;
	//sgbm.speckleRange = 32;
	//sgbm.disp12MaxDiff = 1;
	//sgbm.fullDP = 1;
	//sgbm(ll, rr, disp);

	//disp.convertTo(disp8, CV_8U, 255 / (64 * 16.));
	//namedWindow("opencv_example");
	//imshow("opencv_example", disp8);
	//waitKey(0);
	//destroyWindow("opencv_example");
	

	//std::cin.get();
	return 0;
}