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
	//RETR_EXTERNAL ֻ�������Χ��������������Χ�����ڵ���Χ����������; CHAIN_APPROX_SIMPLE �����������Ĺյ���Ϣ�������������յ㴦�ĵ㱣����contours�����ڣ��յ���յ�֮��ֱ�߶��ϵ���Ϣ�㲻�豣��
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
			//arcLength����ͼ���������ܳ�
			float peri = arcLength(contours[i], true);
			//InputArray curve : һ������ͼ�����������ɵĵ㼯; OutputArray approxCurve����ʾ����Ķ���ε㼯; double epsilon����Ҫ��ʾ����ľ��ȣ��������������֮������������5, 6, 7����8����, , ��; bool closed����ʾ����Ķ�����Ƿ���
			approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
			//drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			////�ǵ����괫�� boundRect
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
	///////////////////////////////////Ԥ����
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//��˹ģ��
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	//��Ե���
	Canny(imgBlur, imgCanny, 25, 75);
	//��ʴerode()������dilate()
	//c++ ��û�� numpy�����ʹ�����·�ʽ����kernal��Ϊdilate() ʹ��
	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDia, kernal);

	return imgDia;
}

vector<Point> recoder(vector<Point> points) 
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;
	//x,y��� ������С�ķֱ������º�����
	//x,y��� ������С�ķֱ������Ϻ�����
	for (int i = 0; i < points.size(); i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}
	//��Сֵ/���ֵ���±�
	//����
	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	//����
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	//����
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	//����
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points,float w, float h)
{
	Mat matrix;
	Point2f src[4] = { points[0],points[1],points[2],points[3] };//�ĸ���
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
	//getPerspectiveTransform ͸�ӱ任
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

void main() {
		string path = "Resources/paper.jpg";
		img = imread(path);
		//�ü� ǰ����
		//resize(img, img, Size(), 0.5, 0.5);

		imgThre = preProcessing(img);
		//��ȡ�ĸ��ǵ�����
		biggest = getContours(imgThre);
		//cout << biggest << endl;
		//drawPoints(biggest, Scalar(0, 0, 255));

		//�ĸ�����������
		docPoints = recoder(biggest);
		//drawPoints(docPoints, Scalar(0, 0, 255));

		//͸�ӱ任
		imgWarp = getWarp(img, docPoints, w, h);

		//�ü�
		int cropVal = 5;
		Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
		imgCrop = imgWarp(roi);

		imshow("Image", img);
		imshow("Imge1", imgThre);
		imshow("Imge Warp", imgWarp);
		imshow("Img Crop", imgCrop);

		waitKey(0);

}
