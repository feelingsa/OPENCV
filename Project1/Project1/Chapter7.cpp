#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat imgGray, imgBlur, imgCanny, imgDia, imgErode;

void getContours(Mat imgDia, Mat img) {
	vector<vector<Point>> contours; 
	vector<Vec4i> hierarchy;
	//RETR_EXTERNAL 只检测最外围轮廓，包含在外围轮廓内的内围轮廓被忽略; CHAIN_APPROX_SIMPLE 仅保存轮廓的拐点信息，把所有轮廓拐点处的点保存入contours向量内，拐点与拐点之间直线段上的信息点不予保留
	findContours(imgDia, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	string objectType;

	for (int i = 0; i < contours.size(); i++) 
	{
		double area = contourArea(contours[i]);
		if (area > 1000) 
		{
			//arcLength计算图像轮廓的周长
			float peri = arcLength(contours[i], true);
			//InputArray curve : 一般是由图像的轮廓点组成的点集; OutputArray approxCurve：表示输出的多边形点集; double epsilon：主要表示输出的精度，就是另个轮廓点之间最大距离数，5, 6, 7，，8，，, , ，; bool closed：表示输出的多边形是否封闭
			approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			//角的坐标传入 boundRect
			boundRect[i] = boundingRect(conPoly[i]);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

			int objCor = (int)conPoly[i].size();
			if (objCor == 3) { objectType = "Thri"; }
			else if (objCor == 4) 
			{ 
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				if (0.95 < aspRatio && aspRatio < 1.05)
				{
					objectType = "Square";
				}
				else
				{
					objectType = "Rect";
				}
			}
			else if (objCor > 4) { objectType = "Circlr"; }

			putText(img, objectType, { boundRect[i].x, boundRect[i].y -5}, FONT_HERSHEY_COMPLEX, 0.5, Scalar(122, 21, 32));
		}
		//cout << area << endl;
	}
}
void main() {
	String path = "Resources/shapes.png";
	Mat img = imread(path);
	

	

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//高斯模糊
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	//边缘检测
	Canny(imgBlur, imgCanny, 25, 75);
	//腐蚀erode()和膨胀dilate()
	//c++ 中没有 numpy，因此使用如下方式创建kernal，为dilate() 使用
	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDia, kernal);

	getContours(imgDia, img);

	imshow("Img", img);
	//imshow("Img Gray", imgGray);
	//imshow("Img Blur", imgBlur);
	//imshow("Img Canny", imgCanny);
	//imshow("Img Dia", imgDia);



	waitKey(0);






}