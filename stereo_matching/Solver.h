#pragma once

#include <iostream>
#include <core/core.hpp>
#include <highgui/highgui.hpp>

#include "Cost.h"


using namespace cv;

const unsigned int MAX_DISP = 128;

class Solver
{
public:
	Solver();
	void Show_disp();
	virtual void Process(Mat &ll, Mat &rr);
	virtual ~Solver();

protected:
	Mat disp, disp_float, disp_float_colored;

};

