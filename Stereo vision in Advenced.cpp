#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>

using namespace std;
using namespace cv;

/*depth_map.txt ����*/
void depth_map_processing()
{
	char processing[1080][1920]; //FHD ����
	char tmp;

	/*depth_map.txt input*/
	ifstream findep("depth_map/depth_map.txt");
	if (!findep) {
		cerr << "Cannot open depth_map.txt for read." << endl;
		return;
	}

	for (int i = 0; i < 1080; i++) {
		for (int j = 0; j < 1920; j++) {
			findep >> tmp;
			if (tmp <= 255 && 0 < tmp) findep >> processing[i][j];
			else if (processing[i][j] == ';') break;
			else i = i - 1;
		}
	}

	/*depth_map_processing.txt output*/
	ofstream foutpro("depth_map/depth_map_processing.txt");
	if (!foutpro) {
		cerr << "Cannot open depth_map_processing.txt for write." << endl;
		return;
	}
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
	
	/*depth_map.txt ����*/
	//depth_map_processing();
}

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

	depth_map(leftimg, rightimg);//Depth_map ����

	return 0;
}