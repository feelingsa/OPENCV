#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;



////////��ɫ���///////
Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 197, smax = 255, vmax = 255;

void main() {
	// 0 �豸��
	VideoCapture cap(0);
	Mat img;

	namedWindow("��ɫѡ��", (640, 200)); //�������ƣ����ڴ�С
	createTrackbar("Hue Min", "��ɫѡ��", &hmin, 179);//179��h�����Χ
	createTrackbar("Hue Max", "��ɫѡ��", &hmax, 179);

	createTrackbar("Sat Min", "��ɫѡ��", &smin, 255);
	createTrackbar("Sat Max", "��ɫѡ��", &smax, 255);

	createTrackbar("Val Min", "��ɫѡ��", &vmin, 255);
	createTrackbar("Val Max", "��ɫѡ��", &vmax, 255);

	while (true)
	{
		cap.read(img);

		//��ɫ�ռ�ת��
		cvtColor(img, imgHSV, COLOR_BGR2HSV);

		Scalar lower(hmin, smin, vmin);
		Scalar uper(hmax, smax, vmax);
		cout << hmin<<","<<smin<<","<<vmin<<","<<hmax<<","<<smax<<","<<vmax<< endl;
		//��ֵ��
		inRange(imgHSV, lower, uper, mask);
		imshow("Imge", img);
		imshow("Imge HSV", imgHSV);
		imshow("Imge  Mask", mask);
		waitKey(1);
	}

	//0, 118, 146, 24, 255, 255




}