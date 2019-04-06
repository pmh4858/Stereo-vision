#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat left = imread("image/leftcamera.jpg", IMREAD_COLOR);
	Mat right = imread("image/rightcamera.jpg", IMREAD_COLOR);
	Mat vision;

	hconcat(left, right, vision);

	imshow("vision", vision);
	waitKey(0);
	return 0;
}