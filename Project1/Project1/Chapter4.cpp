#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));
	//Բ circle(ͼƬ�� Բ��λ�ã� �뾶�� ��ɫ����ϸ)
	circle(img, Point(256, 256), 120, Scalar(231, 13, 32), FILLED);
	//������ rectangle( ������ ���Ͻ����꣬ ���½����꣬ ��ɫ�� ��ϸ)
	rectangle(img, Point(220, 100), Point(230, 120), Scalar(122, 21, 32), 3);
	//�� rectangle(������ ���Ͻ����꣬ ���½����꣬ ��ɫ�� ��ϸ)
	line(img, Point(100, 21), Point(321, 23), Scalar(0, 325, 455),3);

	//�ı�
	putText(img, " Hello!", Point(213, 67), FONT_HERSHEY_COMPLEX, 1, Scalar(122, 21, 32));
	imshow("Imge", img);




	waitKey(0);

}