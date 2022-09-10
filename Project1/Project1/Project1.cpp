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
	//RETR_EXTERNAL 只检测最外围轮廓，包含在外围轮廓内的内围轮廓被忽略; CHAIN_APPROX_SIMPLE 仅保存轮廓的拐点信息，把所有轮廓拐点处的点保存入contours向量内，拐点与拐点之间直线段上的信息点不予保留
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
			//arcLength计算图像轮廓的周长
			float peri = arcLength(contours[i], true);
			//InputArray curve : 一般是由图像的轮廓点组成的点集; OutputArray approxCurve：表示输出的多边形点集; double epsilon：主要表示输出的精度，就是另个轮廓点之间最大距离数，5, 6, 7，，8，，, , ，; bool closed：表示输出的多边形是否封闭
			approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
			//drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			//角的坐标传入 boundRect
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
	////////颜色检测///////
	Mat imgHSV;
	//彩色空间转换
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < mycolors.size(); i++)
	{
		Scalar lower(mycolors[i][0], mycolors[i][1], mycolors[i][2]);
		Scalar uper(mycolors[i][3], mycolors[i][4], mycolors[i][5]);
		//二值化
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
	// 0 设备号
	VideoCapture cap(0);


	while (true)
	{
		cap.read(img);

		//找颜色
		vector<vector<int>> newpoints = findColor(img);
		drawOnCanvas(newPoints, mycolorValue);

		imshow("Image", img);
		waitKey(1);
	}

}