#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat imgWarp, matrix;
float w = 250, h = 350;
void main() {
	String path = "Resources/cards.jpg";
	Mat img = imread(path);

	Point2f src[4] = { {529,142},{771,190},{405,395},{674,457} };//添加扑克牌的四个点
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
	//getPerspectiveTransform 透视变换
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	for (int i = 0 ; i < 4; i++) {
		circle(img, src[i], 10, Scalar(231, 13, 32), FILLED);
	}

	imshow("Imge", img);
	imshow("Imge Warp", imgWarp);



	waitKey(0);

}