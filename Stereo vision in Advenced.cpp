#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>

using namespace std;
using namespace cv;

Mat leftimg, rightimg;

/*Depth_map ���� �޼ҵ�*/
void depth_map(Mat leftimg, Mat rightimg)
{
	Size imgsz = leftimg.size(); //�̹��� ������

	Mat disp_left = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat disp_right = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat gl, gr, disp, disp8;

	/*�̹��� �׷��̽����Ϸ� ��ȯ*/
	cvtColor(leftimg, gl, COLOR_BGR2GRAY);
	cvtColor(rightimg, gr, COLOR_BGR2GRAY);

	/*StereoBM = Depth_map ����*/
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

	/*StereoBM���� ���� ���� �̿��ؼ� �̹��� ��ֶ�����*/
	normalize(disp_left, disp8, 0, 255, CV_MINMAX, CV_8U);

	/*Depth_map �̹��� �ۼ�*/
	imwrite("image/depth_map.png", disp8);

	/*Depth_map �̹��� ���*/
	namedWindow("depth map", WINDOW_AUTOSIZE);
	imshow("depth map", disp8);

	waitKey(0);

	return;
}

int main()
{
	leftimg = imread("image/leftimg2.jpg", IMREAD_COLOR); //���� ī�޶�
	rightimg = imread("image/rightimg2.jpg", IMREAD_COLOR); //���� ī�޶�

	depth_map(leftimg, rightimg);//Depth_map ����

	return 0;
}