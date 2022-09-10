#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDia, imgErode;

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//��˹ģ��
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	//��Ե���
	Canny(imgBlur, imgCanny, 25, 75);
	//��ʴerode()������dilate()
	//c++ ��û�� numpy�����ʹ�����·�ʽ����kernal��Ϊdilate() ʹ��
	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDia, kernal);
	erode(imgDia, imgErode, kernal);
	//imshow("Imge", img);
	//imshow("Imge Gray", imgGray);
	//imshow("Imge Blur", imgBlur);
	imshow("Imge Canny", imgCanny);
	imshow("Imge Dia", imgDia);
	imshow("Imge Erode", imgErode);

	waitKey(0);

}