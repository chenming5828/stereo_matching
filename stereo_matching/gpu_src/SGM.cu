#include "../gpu_inc/SGM.cuh"


GPU_SGM::GPU_SGM()
{
	cudaSetDevice(0);
	cudaMalloc((void**)&d_ll, IMG_H* IMG_W * sizeof(uchar));
	cudaMalloc((void**)&d_rr, IMG_H * IMG_W * sizeof(uchar));
	cudaMalloc((void**)&d_disp, IMG_H * IMG_W * sizeof(uchar));	
	cudaMalloc((void**)&d_cost_table_l, IMG_H * IMG_W * CU_MAX_DISP * sizeof(uint64_t));
	cudaMalloc((void**)&d_cost_table_r, IMG_H * IMG_W * CU_MAX_DISP * sizeof(uint64_t));
	cudaMalloc((void**)&d_cost, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));

	cudaMalloc((void**)&d_L1, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_L2, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_L3, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_L4, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_min_L1, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_min_L1, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_min_L1, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	cudaMalloc((void**)&d_min_L1, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	if (CU_USE_8_PATH)
	{
		cudaMalloc((void**)&d_L5, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_L6, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_L7, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_L8, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_min_L5, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_min_L6, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_min_L7, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
		cudaMalloc((void**)&d_min_L8, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float));
	}

	P1 = 10;
	P2 = 100;
}



GPU_SGM::~GPU_SGM()
{
	cudaFree(d_ll);
	cudaFree(d_rr);
	cudaFree(d_disp);
	cudaFree(d_cost_table_l);
	cudaFree(d_cost_table_r);
	cudaFree(d_cost);

	cudaFree(d_L1);
	cudaFree(d_L2);
	cudaFree(d_L3);
	cudaFree(d_L4);
	cudaFree(d_min_L1);
	cudaFree(d_min_L2);
	cudaFree(d_min_L3);
	cudaFree(d_min_L4);
	if (CU_USE_8_PATH)
	{
		cudaFree(d_L5);
		cudaFree(d_L6);
		cudaFree(d_L7);
		cudaFree(d_L8);
		cudaFree(d_min_L5);
		cudaFree(d_min_L6);
		cudaFree(d_min_L7);
		cudaFree(d_min_L8);
	}
}


void GPU_SGM::Process(Mat &ll, Mat &rr, Mat &disp, float *cost)
{
	cudaSetDevice(0);
	cudaMemcpy(d_ll, ll.data, IMG_H* IMG_W * sizeof(uchar), cudaMemcpyHostToDevice);
	cudaMemcpy(d_rr, rr.data, IMG_H* IMG_W * sizeof(uchar), cudaMemcpyHostToDevice);

	dim3 grid, block;
	grid.x = (IMG_W - 1) / 32 + 1;
	grid.y = (IMG_H - 1) / 32 + 1;
	block.x = 32;
	block.y = 32;

	cu_Build_cost_table<<<grid, block>>>(d_ll, d_rr, 
		                                                                 d_cost_table_l, 
		                                                                 d_cost_table_r, 
		                                                                 IMG_W, IMG_H, 
		                                                                 CU_WIN_W, CU_WIN_H);
	printf("cost table finished\n");
	cu_Build_dsi_from_table<<<grid, block>>>(d_cost_table_l, 
		                                                                          d_cost_table_r, 
		                                                                          d_cost,
		                                                                          IMG_W, IMG_H, CU_MAX_DISP);
	cudaDeviceSynchronize();
	printf("dsi finished\n");

	cudaMemcpy(cost, d_cost, IMG_H * IMG_W * CU_MAX_DISP * sizeof(float), cudaMemcpyDeviceToHost);
}