#include "../global.h"


__global__ void cu_Build_cost_table(uchar *d_ll, uchar *d_rr,
																   uint64_t *d_cost_table_l,
																   uint64_t *d_cost_table_r,
																   int img_w, int img_h,
																   int win_w, int win_h);
__global__ void cu_Build_dsi_from_table(uint64_t *d_cost_table_l,
																		   uint64_t *d_cost_table_r,
																		   float *d_cost,
																		   int img_w, int img_h, int max_disp);
__device__ int cu_hamming_cost(uint64_t ct_l, uint64_t ct_r);
