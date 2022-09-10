#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img;
vector<vector<int>> mycolors{ {0, 118, 146, 24, 255, 255},{158,107,129,172,252,255} };
//(0, 118, 146, 24, 255, 255) (158,107,129,172,252,255)
vector<Scalar> mycolorValue{ {255,215,0},{255,105,180} };
vector<vector<int>> newPoints;


Point getContours(Mat imgDia, Mat img) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//RETR_EXTERNAL ֻ�������Χ��������������Χ�����ڵ���Χ����������; CHAIN_APPROX_SIMPLE �����������Ĺյ���Ϣ�������������յ㴦�ĵ㱣����contours�����ڣ��յ���յ�֮��ֱ�߶��ϵ���Ϣ�㲻�豣��
	findContours(imgDia, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	string objectType;

	Point mypoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > 1000)
		{
			//arcLength����ͼ���������ܳ�
			float peri = arcLength(contours[i], true);
			//InputArray curve : һ������ͼ�����������ɵĵ㼯; OutputArray approxCurve����ʾ����Ķ���ε㼯; double epsilon����Ҫ��ʾ����ľ��ȣ��������������֮������������5, 6, 7����8����, , ��; bool closed����ʾ����Ķ�����Ƿ���
			approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
			//drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			//�ǵ����괫�� boundRect
			boundRect[i] = boundingRect(conPoly[i]);
			mypoint.x = boundRect[i].x + boundRect[i].width / 2;
			mypoint.y = boundRect[i].y;

		}
		//cout << area << endl;
	}
	return mypoint;
}
void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> mycolorValue)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, (Point(newPoints[i][0],newPoints[i][1])),5, mycolorValue[newPoints[i][2]], FILLED);
	}
}
vector<vector<int>> findColor(Mat img)
{
	////////��ɫ���///////
	Mat imgHSV;
	//��ɫ�ռ�ת��
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < mycolors.size(); i++)
	{
		Scalar lower(mycolors[i][0], mycolors[i][1], mycolors[i][2]);
		Scalar uper(mycolors[i][3], mycolors[i][4], mycolors[i][5]);
		//��ֵ��
		Mat mask;
		inRange(imgHSV, lower, uper, mask);
		Point mypoint = getContours(mask, img);
		if (mypoint.x != 0 && mypoint.y != 0)
		{
			newPoints.push_back({ mypoint.x, mypoint.y, i });
		}
		
		//imshow(to_string(i), mask);
	}

	//imshow("Imge", img);
	//imshow("Imge HSV", imgHSV);
	
	return newPoints;

}



void main() {
	// 0 �豸��
	VideoCapture cap(0);


	while (true)
	{
		cap.read(img);

		//����ɫ
		vector<vector<int>> newpoints = findColor(img);
		drawOnCanvas(newPoints, mycolorValue);

		imshow("Image", img);
		waitKey(1);
	}

}