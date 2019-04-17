#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>

using namespace std;
using namespace cv;

/*depth_map ����*/
void depth_map_processing()
{
	/*���� �ڵ��ϸ� �˴ϴ�.*/
}

/*Depth_map �ؽ�Ʈ ���� ���*/
void depth_map_stream(Mat disp8)
{
	/*depth_map.txt output*/
	ofstream foutdep("depth_map/depth_map.txt");
	if (!foutdep) {
		cerr << "Cannot open depth_map.txt for write." << endl;
		return;
	}

	foutdep << disp8;
	
	/*depth_map ����*/
	//depth_map_processing();
}

/*Depth_map SGBM ���� �޼ҵ�*/
void depth_map_SGBM(Mat leftimg, Mat rightimg)
{
	Size imgsz = leftimg.size(); //�̹��� ������

	Mat disp_left = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat disp_right = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat gl, gr, disp, disp8;

	/*�̹��� �׷��̽����Ϸ� ��ȯ*/
	cvtColor(leftimg, gl, COLOR_BGR2GRAY);
	cvtColor(rightimg, gr, COLOR_BGR2GRAY);

	/*StereoSGBM = Depth_map ����*/
	Ptr<cv::StereoSGBM> sgbm = StereoSGBM::create();
	sgbm->setMinDisparity(-39); //Default -39
	sgbm->setNumDisparities(32); //Default 16
	sgbm->setBlockSize(21); //Default 21
	sgbm->setP1(216); //Default 216
	sgbm->setP2(864); //Default 864
	sgbm->setDisp12MaxDiff(1); //Default 1
	sgbm->setPreFilterCap(63); //Default 61
	sgbm->setUniquenessRatio(10); //Default 0
	sgbm->setSpeckleWindowSize(100); //Default 100
	sgbm->setSpeckleRange(32); //Default 32
	sgbm->setMode(StereoSGBM::MODE_SGBM); //Default MOD_SGBM
	sgbm->compute(gl, gr, disp_left);

	/*StereoBM���� ���� ���� �̿��ؼ� �̹��� ��ֶ�����*/
	normalize(disp_left, disp8, 0, 255, CV_MINMAX, CV_8U);

	/*Depth_map �̹��� �ۼ�*/
	imwrite("depth_map/depth_map.png", disp8);

	/*Depth_map �ؽ�Ʈ ���*/
	depth_map_stream(disp8);

	/*Depth_map �̹��� ���*/
	namedWindow("depth map", WINDOW_AUTOSIZE);
	imshow("depth map", disp8);

	waitKey(0);

	return;
}

/*Depth_map BM ���� �޼ҵ�*/
void depth_map_BM(Mat leftimg, Mat rightimg)
{
	Size imgsz = leftimg.size(); //�̹��� ������

	Mat disp_left = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat disp_right = Mat(imgsz.height, imgsz.width, CV_16S);
	Mat gl, gr, disp, disp8;

	/*�̹��� �׷��̽����Ϸ� ��ȯ*/
	cvtColor(leftimg, gl, COLOR_BGR2GRAY);
	cvtColor(rightimg, gr, COLOR_BGR2GRAY);

	/*StereoBM = Depth_map ����*/
	Ptr<cv::StereoBM> sbm = StereoBM::create(0, 21); //Default (0, 21)

	sbm->setDisp12MaxDiff(1); //Default 1
	sbm->setSpeckleRange(8); //Default 8
	sbm->setSpeckleWindowSize(9); //Default 9
	sbm->setUniquenessRatio(0); //Default 0
	sbm->setTextureThreshold(850); //Default 507
	sbm->setMinDisparity(-39); //Default -39
	sbm->setPreFilterCap(63); //Default 61
	sbm->setPreFilterSize(5); //Default 5
	sbm->compute(gl, gr, disp_left);

	/*StereoBM���� ���� ���� �̿��ؼ� �̹��� ��ֶ�����*/
	normalize(disp_left, disp8, 0, 255, CV_MINMAX, CV_8U);

	/*Depth_map �̹��� �ۼ�*/
	imwrite("depth_map/depth_map.png", disp8);

	/*Depth_map �ؽ�Ʈ ���*/
	depth_map_stream(disp8);

	/*Depth_map �̹��� ���*/
	namedWindow("depth map", WINDOW_AUTOSIZE);
	imshow("depth map", disp8);

	waitKey(0);

	return;
}

int main()
{
	Mat leftimg, rightimg;

	leftimg = imread("image/leftimg2.jpg", IMREAD_COLOR); //���� ī�޶�
	rightimg = imread("image/rightimg2.jpg", IMREAD_COLOR); //���� ī�޶�

	depth_map_BM(leftimg, rightimg);//Depth_map ����

	return 0;
}