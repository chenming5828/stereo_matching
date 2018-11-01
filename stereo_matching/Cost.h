#pragma once

#include "global.h"
#include "Solver.h"


float SAD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w, float* weight);
float SSD(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w, float* weight);

uint16_t CT(Mat &ll, Mat &rr, Point l_pt, uchar disp, uchar win_h, uchar win_w, float* weight);
uint16_t hamming_cost(uint64_t ct_l, uint64_t ct_r);

uint64_t CT_pts(Mat &im, int u, int v, uchar win_h, uchar win_w, float* weight);