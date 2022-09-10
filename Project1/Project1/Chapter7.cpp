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
	//RETR_EXTERNAL ֻ�������Χ��������������Χ�����ڵ���Χ����������; CHAIN_APPROX_SIMPLE �����������Ĺյ���Ϣ�������������յ㴦�ĵ㱣����contours�����ڣ��յ���յ�֮��ֱ�߶��ϵ���Ϣ�㲻�豣��
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
			//arcLength����ͼ���������ܳ�
			float peri = arcLength(contours[i], true);
			//InputArray curve : һ������ͼ�����������ɵĵ㼯; OutputArray approxCurve����ʾ����Ķ���ε㼯; double epsilon����Ҫ��ʾ����ľ��ȣ��������������֮������������5, 6, 7����8����, , ��; bool closed����ʾ����Ķ�����Ƿ���
			approxPolyDP(contours[i], conPoly[i], 0.02*peri, true);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			//�ǵ����괫�� boundRect
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
	//��˹ģ��
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	//��Ե���
	Canny(imgBlur, imgCanny, 25, 75);
	//��ʴerode()������dilate()
	//c++ ��û�� numpy�����ʹ�����·�ʽ����kernal��Ϊdilate() ʹ��
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