#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));
	//圆 circle(图片， 圆心位置， 半径， 颜色，粗细)
	circle(img, Point(256, 256), 120, Scalar(231, 13, 32), FILLED);
	//长方形 rectangle( 背景， 左上角坐标， 右下角坐标， 颜色， 粗细)
	rectangle(img, Point(220, 100), Point(230, 120), Scalar(122, 21, 32), 3);
	//线 rectangle(背景， 左上角坐标， 右下角坐标， 颜色， 粗细)
	line(img, Point(100, 21), Point(321, 23), Scalar(0, 325, 455),3);

	//文本
	putText(img, " Hello!", Point(213, 67), FONT_HERSHEY_COMPLEX, 1, Scalar(122, 21, 32));
	imshow("Imge", img);




	waitKey(0);

}