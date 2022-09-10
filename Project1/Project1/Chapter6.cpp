#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;



////////��ɫ���///////
Mat imgHSV, mask;
int hmin = 0, smin = 110, vmin = 153;
int hmax = 19, smax = 240, vmax = 255;

void main() {
	String path = "Resources/shapes.png";
	Mat img = imread(path);
	
	//��ɫ�ռ�ת��
	cvtColor(img, imgHSV, COLOR_BGR2HSV);


	namedWindow("��ɫѡ��", (640, 200)); //�������ƣ����ڴ�С
	createTrackbar("Hue Min", "��ɫѡ��", &hmin, 179);//179��h�����Χ
	createTrackbar("Hue Max", "��ɫѡ��", &hmax, 179);

	createTrackbar("Sat Min", "��ɫѡ��", &smin, 255);
	createTrackbar("Sat Max", "��ɫѡ��", &smax, 255);

	createTrackbar("Val Min", "��ɫѡ��", &vmin, 255);
	createTrackbar("Val Max", "��ɫѡ��", &vmax, 255);

	while (true) {
		Scalar lower(hmin, smin, vmin);
		Scalar uper(hmax, smax, vmax);
		//��ֵ��
		inRange(imgHSV, lower, uper, mask);
		imshow("Imge", img);
		imshow("Imge HSV", imgHSV);
		imshow("Imge  Mask", mask);
		waitKey(1);
	}




	

}