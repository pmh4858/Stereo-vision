#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>

using namespace std;
using namespace cv;

Mat leftimg, rightimg;

void depth_map(Mat leftimg, Mat rightimg)
{
	Size imgsz = leftimg.size(); //이미지 사이즈

	Mat disp_left = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat disp_right = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat gl, gr, disp, disp8;

	cvtColor(leftimg, gl, COLOR_BGR2GRAY);
	cvtColor(rightimg, gr, COLOR_BayerBG2GRAY);

	Ptr<cv::StereoBM> sbm = StereoBM::create(0, 21);

	sbm->setDisp12MaxDiff(1);
	sbm->setSpeckleRange(8);
	sbm->setSpeckleWindowSize(9);
	sbm->setUniquenessRatio(0);
	sbm->setTextureThreshold(507);
	sbm->setMinDisparity(-39);
	sbm->setPreFilterCap(61);
	sbm->setPreFilterSize(5);
	sbm->compute(gl, gr, disp_left);

	normalize(disp_left, disp8, 0, 255, NORM_MINMAX, CV_8U);

	imwrite("image/depth_map.png", disp8);

	namedWindow("depth map", WINDOW_AUTOSIZE);
	imshow("depth map", disp8);

	waitKey(0);

	return;
}

int main()
{
	leftimg = imread("image/leftcamera.jpg", IMREAD_COLOR); //왼측 카메라
	rightimg = imread("image/rightcamera.jpg", IMREAD_COLOR); //우측 카메라

	depth_map(leftimg, rightimg);

	return 0;
}