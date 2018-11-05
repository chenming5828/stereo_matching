#include "../gpu_inc/cost.cuh"


__global__ void cu_Build_cost_table(uchar *d_ll, uchar *d_rr,
																   uint64_t *d_cost_table_l, 
	                                                               uint64_t *d_cost_table_r,
	                                                               int img_w, int img_h,
																   int win_w, int win_h)
{
	int index = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x * blockDim.y + threadIdx.y * blockDim.x + threadIdx.x;
	if (index > img_w * img_h - 1)  return;
	int col = index % img_w;
	int row = index / img_w;

	uint64_t value_l = 0, value_r = 0;
	uchar ctr_pixel_l = d_ll[row*img_w + col];
	uchar ctr_pixel_r = d_rr[row*img_w + col];

	for (int i = -win_h / 2; i <= win_h / 2; i++)
	{
		int y = MAX(row + i, 0);		// check border
		y = MIN(y, img_h - 1);
		for (int j = -win_w / 2; j <= win_w / 2; j++)
		{
			if (i == 0 && j == 0)
				continue;
			int x = MAX(col + j, 0);
			x = MIN(x, img_w - 1);
			int index_ = y * img_w + x;
			value_l = (value_l | (d_ll[index_] > ctr_pixel_l)) << 1;
			value_r = (value_r | (d_rr[index_] > ctr_pixel_r)) << 1;
		}
	}
	d_cost_table_l[row*img_w + col] = value_l;
	d_cost_table_r[row*img_w + col] = value_r;
	return;
}


__global__ void cu_Build_dsi_from_table(uint64_t *d_cost_table_l,
																		   uint64_t *d_cost_table_r,
																		   float *d_cost,
																		   int img_w, int img_h, int max_disp)
{
	int index = (blockIdx.y * gridDim.x + blockIdx.x) * blockDim.x * blockDim.y + threadIdx.y * blockDim.x + threadIdx.x;
	if (index > img_w * img_h - 1)  return;
	int col = index % img_w;
	int row = index / img_w;

	for (int d = 0; d < max_disp; d++)
	{
		int dst_index = row * img_w * max_disp + col * max_disp + d;
		uint64_t ct_l = d_cost_table_l[row*img_w + col];
		uint64_t ct_r = d_cost_table_r[row*img_w + MAX(col - d, 0)];
		d_cost[dst_index] = cu_hamming_cost(ct_l, ct_r);
	}
}


__device__ int cu_hamming_cost(uint64_t ct_l, uint64_t ct_r)
{
	uint64_t not_the_same = ct_l ^ ct_r;
	// find the number of '1', log(N)
	int cnt = 0;
	while (not_the_same)
	{
		//std::cout << not_the_same << std::endl;
		cnt += (not_the_same & 1);
		not_the_same >>= 1;
	}
	return cnt;
}