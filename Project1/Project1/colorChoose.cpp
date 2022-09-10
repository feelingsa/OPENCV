#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;



////////颜色检测///////
Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 0;
int hmax = 197, smax = 255, vmax = 255;

void main() {
	// 0 设备号
	VideoCapture cap(0);
	Mat img;

	namedWindow("颜色选择", (640, 200)); //窗口名称，窗口大小
	createTrackbar("Hue Min", "颜色选择", &hmin, 179);//179：h的最大范围
	createTrackbar("Hue Max", "颜色选择", &hmax, 179);

	createTrackbar("Sat Min", "颜色选择", &smin, 255);
	createTrackbar("Sat Max", "颜色选择", &smax, 255);

	createTrackbar("Val Min", "颜色选择", &vmin, 255);
	createTrackbar("Val Max", "颜色选择", &vmax, 255);

	while (true)
	{
		cap.read(img);

		//彩色空间转换
		cvtColor(img, imgHSV, COLOR_BGR2HSV);

		Scalar lower(hmin, smin, vmin);
		Scalar uper(hmax, smax, vmax);
		cout << hmin<<","<<smin<<","<<vmin<<","<<hmax<<","<<smax<<","<<vmax<< endl;
		//二值化
		inRange(imgHSV, lower, uper, mask);
		imshow("Imge", img);
		imshow("Imge HSV", imgHSV);
		imshow("Imge  Mask", mask);
		waitKey(1);
	}

	//0, 118, 146, 24, 255, 255




}