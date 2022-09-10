#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img, imgThre, imgGray, imgBlur, imgCanny, imgDia, imgWarp, imgCrop;
vector<Point> biggest, docPoints;
float w = 420, h = 596;

vector<Point> getContours(Mat img) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//RETR_EXTERNAL 只检测最外围轮廓，包含在外围轮廓内的内围轮廓被忽略; CHAIN_APPROX_SIMPLE 仅保存轮廓的拐点信息，把所有轮廓拐点处的点保存入contours向量内，拐点与拐点之间直线段上的信息点不予保留
	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	vector<Point> biggest;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		string objectType;

		if (area > 1000)
		{
			//arcLength计算图像轮廓的周长
			float peri = arcLength(contours[i], true);
			//InputArray curve : 一般是由图像的轮廓点组成的点集; OutputArray approxCurve：表示输出的多边形点集; double epsilon：主要表示输出的精度，就是另个轮廓点之间最大距离数，5, 6, 7，，8，，, , ，; bool closed：表示输出的多边形是否封闭
			approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
			//drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			////角的坐标传入 boundRect
			//boundRect[i] = boundingRect(conPoly[i]);

			cout << (int)conPoly[i].size() << endl;
			if (area > maxArea && (int)conPoly[i].size()==4)
			{
				maxArea = area; 
				biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
			}
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
		}
	}
	return biggest;
}

void drawPoints(vector<Point> points, Scalar color)
{
	for (int i = 0; i < points.size(); i++)
	{
		circle(img, points[i], 10, color, FILLED);
		putText(img, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);

	}
}

Mat preProcessing(Mat img)
{
	///////////////////////////////////预处理
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//高斯模糊
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	//边缘检测
	Canny(imgBlur, imgCanny, 25, 75);
	//腐蚀erode()和膨胀dilate()
	//c++ 中没有 numpy，因此使用如下方式创建kernal，为dilate() 使用
	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDia, kernal);

	return imgDia;
}

vector<Point> recoder(vector<Point> points) 
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;
	//x,y相加 最大和最小的分别是右下和左上
	//x,y相减 最大和最小的分别是右上和左下
	for (int i = 0; i < points.size(); i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}
	//最小值/最大值的下标
	//左上
	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	//右上
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	//左下
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	//右下
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points,float w, float h)
{
	Mat matrix;
	Point2f src[4] = { points[0],points[1],points[2],points[3] };//四个点
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
	//getPerspectiveTransform 透视变换
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

void main() {
		string path = "Resources/paper.jpg";
		img = imread(path);
		//裁剪 前期用
		//resize(img, img, Size(), 0.5, 0.5);

		imgThre = preProcessing(img);
		//获取四个角的坐标
		biggest = getContours(imgThre);
		//cout << biggest << endl;
		//drawPoints(biggest, Scalar(0, 0, 255));

		//四个角重新排序
		docPoints = recoder(biggest);
		//drawPoints(docPoints, Scalar(0, 0, 255));

		//透视变换
		imgWarp = getWarp(img, docPoints, w, h);

		//裁剪
		int cropVal = 5;
		Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
		imgCrop = imgWarp(roi);

		imshow("Image", img);
		imshow("Imge1", imgThre);
		imshow("Imge Warp", imgWarp);
		imshow("Img Crop", imgCrop);

		waitKey(0);

}
