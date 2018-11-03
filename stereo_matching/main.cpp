#include "global.h"
#include "GM.h"
#include "SGM.h"
#include "utils.h"

int main()
{
	Mat ll = imread("example/kitti_1_left.png", IMREAD_GRAYSCALE);
	Mat rr = imread("example/kitti_1_right.png", IMREAD_GRAYSCALE);
	std::cout << "rows: " << ll.rows << ", cols: " << ll.cols << std::endl;

	//Solver *sv = new GM(ll, rr);
	Solver *sv = new SGM(ll, rr);

	std::cout << "waiting ... " << std::endl;
	double be = get_cur_ms();
	sv->Process();
	double en = get_cur_ms();
	std::cout << "done ... " << std::endl;
	std::cout << "time cost: " << en - be << " ms" << std::endl;
	sv->Show_disp();
	Mat disp = sv->get_disp();

	delete sv;

	//Mat remap(disp.rows, disp.cols, CV_8UC3, Scalar(0));
	//Mat ll_rgb = imread("example/left_1.png");
	//for (int i = 0; i < remap.rows; i++)
	//{
	//	for (int j = 0; j < remap.cols; j++)
	//	{
	//		if (disp.at<uchar>(i, j) == INVALID_DISP)
	//		{
	//			remap.at<Vec3b>(i, j)[0] = 0;
	//			remap.at<Vec3b>(i, j)[1] = 0;
	//			remap.at<Vec3b>(i, j)[2] = 0;
	//		}
	//		else
	//		{
	//			remap.at<Vec3b>(i, j)[0] = ll_rgb.at<Vec3b>(i, j)[0];
	//			remap.at<Vec3b>(i, j)[1] = ll_rgb.at<Vec3b>(i, j)[1];
	//			remap.at<Vec3b>(i, j)[2] = ll_rgb.at<Vec3b>(i, j)[2];
	//		}
	//	}
	//}
	//namedWindow("stereo_remap");
	//imshow("stereo_remap", remap);
	//waitKey();

	//Mat disp = imread("example/disp_grey_1.png", IMREAD_GRAYSCALE);
	//Mat ll_rgb = imread("example/left_1.png");
	//Mat disp_float(disp.rows, disp.cols, CV_32FC1, Scalar(0));

	//for (int i = 0; i < disp.rows; i++)
	//{
	//	uchar *ptr = disp.ptr<uchar>(i);
	//	float *ptr2 = disp_float.ptr<float>(i);
	//	for (int j = 0; j < disp.cols; j++)
	//	{
	//		if (j > 0 && j < disp.cols - 1 && ptr[j] < 128 && ptr[j - 1] < 128 && ptr[j + 1] < 128 && ptr[j - 1] - 2 * ptr[j] + ptr[j + 1])
	//		{

	//			ptr2[j] = ptr[i] + (ptr[j - 1] - ptr[j + 1]) / (2 * (ptr[j - 1] - 2 * ptr[j] + ptr[j + 1]));
	//		}
	//		else
	//		{
	//			ptr2[j] = ptr[j];
	//		}
	//		printf("%f, ", ptr2[j]);
	//	}
	//	printf("\n");
	//}
	//std::cin.get();

	//// read calibration
	//std::ifstream in;
	//in.open("example/calib_1.txt");
	//if (!in.is_open()){
	//	std::cout << "reading calib file failed." << std::endl;
	//	std::cin.get();
	//	return 0;
	//}
	//string str, str_tmp;
	//std::stringstream ss;
	////while (std::getline(in, str)){
	////	std::cout << str << std::endl;
	////}
	//std::getline(in, str);  // only read left cam P0
	//ss.clear();
	//ss.str(str);

	//double fx, fy, cx, cy;
	//for (int i = 0; i < 13; i++)
	//{
	//	if (i == 1)
	//	{
	//		ss >> fx;
	//	}
	//	else if (i == 3)
	//	{
	//		ss >> cx;
	//	}
	//	else if (i == 6)
	//	{
	//		ss >> fy;
	//	}
	//	else if (i == 7)
	//	{
	//		ss >> cy;
	//	}
	//	else
	//	{
	//		ss >> str_tmp;
	//	}
	//}
	//printf("P0 intrinsic param: \nfx: %lf, fy: %lf, cx: %lf, cy: %lf\n", fx, fy, cx, cy);

	//// convert disp -> Zc and compute Xc, Yc
	//// +Zc points to front,  +Xc points to right, +Yc points to down
	//double baseline = 0.4;
	//double *Xc = new double[disp.rows * disp.cols];
	//double *Yc = new double[disp.rows * disp.cols];
	//double *Zc = new double[disp.rows * disp.cols];

	//double min_X = DBL_MAX, max_X = DBL_MIN;
	//double min_Y = DBL_MAX, max_Y = DBL_MIN;
	//double min_Z = DBL_MAX, max_Z = DBL_MIN;

	//for (int i = 0; i < disp.rows; i++)
	//{
	//	uchar *ptr = disp.ptr<uchar>(i);
	//	for (int j = 0; j < disp.cols; j++)
	//	{
	//		if (ptr[j] == INVALID_DISP)
	//		{
	//			Zc[i*disp.cols + j] = -1;  // invalid
	//		}
	//		else
	//		{
	//			Zc[i*disp.cols + j] = fx * baseline / (MAX(ptr[j], 3));
	//		}
	//		if (Zc[i*disp.cols + j] > 0)
	//		{
	//			Xc[i*disp.cols + j] = (j - cx) * Zc[i*disp.cols + j] / fx;
	//			Yc[i*disp.cols + j] = (i - cy) * Zc[i*disp.cols + j] / fy;

	//			if (Xc[i*disp.cols + j] > max_X)  max_X = Xc[i*disp.cols + j];
	//			if (Xc[i*disp.cols + j] < min_X)  min_X = Xc[i*disp.cols + j];
	//			if (Yc[i*disp.cols + j] > max_Y)  max_Y = Yc[i*disp.cols + j];
	//			if (Yc[i*disp.cols + j] < min_Y)  min_Y = Yc[i*disp.cols + j];
	//			if (Zc[i*disp.cols + j] > max_Z)  max_Z = Zc[i*disp.cols + j];
	//			if (Zc[i*disp.cols + j] < min_Z)  min_Z = Zc[i*disp.cols + j];
	//		}
	//	}
	//}
	//// fov in camera coords
	//printf("minx: %lf, maxx:%lf\n", min_X, max_X);
	//printf("miny: %lf, maxy:%lf\n", min_Y, max_Y);  //  indicates the camera height from ground
	//printf("minz: %lf, maxz:%lf\n", min_Z, max_Z);

	//float map_scale = 0.1;
	//float front_range = 60, left_right_range = 50;
	//float ground_estimate = 0.5;
	//Mat bird_view(int(front_range / map_scale), int(left_right_range / map_scale), CV_8UC3, Scalar(0));
	//for (int i = 0; i < disp.rows; i++)
	//{
	//	for (int j = 0; j < disp.cols; j++)
	//	{
	//		if (Zc[i*disp.cols + j] > 0 && Zc[i*disp.cols + j] < front_range)
	//		{
	//			if (Yc[i*disp.cols + j] < ground_estimate)
	//			{
	//				int u = MAX(MIN(int((Xc[i*disp.cols + j] + left_right_range/2) / map_scale), bird_view.cols - 1), 0);
	//				int v = MAX(MIN(int((front_range - Zc[i*disp.cols + j]) / map_scale), bird_view.rows - 1), 0);
	//				bird_view.at<Vec3b>(v, u)[0] = ll_rgb.at<Vec3b>(i, j)[0];
	//				bird_view.at<Vec3b>(v, u)[1] = ll_rgb.at<Vec3b>(i, j)[1];
	//				bird_view.at<Vec3b>(v, u)[2] = ll_rgb.at<Vec3b>(i, j)[2];
	//			}
	//		}
	//	}
	//}
	//namedWindow("stereo_pointcloud", 0);
	//imshow("stereo_pointcloud", bird_view);
	//waitKey();

	//// write file for meshlab visualization
	//std::ofstream out;
	//out.open("example/pt_1.txt");
	//for (int i = 0; i < disp.rows; i++)
	//{
	//	for (int j = 0; j < disp.cols; j++)
	//	{
	//		if (Zc[i*disp.cols + j] > 0 && Zc[i*disp.cols + j] < front_range)
	//		{
	//			if (Yc[i*disp.cols + j] < ground_estimate)
	//			{
	//				std::stringstream ss;
	//				string str;
	//				ss << Xc[i*disp.cols + j] << "," << Yc[i*disp.cols + j] << "," << Zc[i*disp.cols + j] << std::endl;
	//				str = ss.str();
	//				out << str;
	//			}
	//		}
	//	}
	//}

	
	//std::cin.get();
	return 0;
}