#pragma once

#include "global.h"


float SAD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w);
float SSD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w);

uint16_t CT(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w);
uint16_t hamming_cost(uint64_t ct_l, uint64_t ct_r);