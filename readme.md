Stereo Matching
==================
This repository is an example for stereo matching I've been learning recently. 

Pre-requisites
--------------
Visual Studio 2015

CUDA

OpenCV

MeshLab

Datasets
--------
The code is tested on [Kitti dataset](http://www.cvlibs.net/datasets/kitti/eval_stereo.php).

Result
-----
Below shows the disparity and coresponding pointcloud mapping visualized using MeshLab.

Now it costs about 3s per img (about 1280 * 375). Supporting gpu support gradually.

![disparity_0.jpg](https://github.com/hunterlew/stereo_matching/blob/master/stereo_matching/example/disp_rgb_0.png)
![mapping_0.jpg](https://github.com/hunterlew/stereo_matching/blob/master/stereo_matching/example/mapping_0.png)