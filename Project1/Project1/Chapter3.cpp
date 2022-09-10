#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	string path = "Resources/test.png";
	Mat img = imread(path);
	Mat imgResize, imgCrop;
	//cout << img.size << endl;
	resize(img, imgResize, Size(), 0.5, 0.5);
	//cout << imgResize.size << endl;

	Rect roi(100, 100, 200, 100);
	imgCrop = img(roi);
	imshow("Imge", img);
	imshow("Imge Resize", imgResize);
	imshow("Imge Crop", imgCrop);



	waitKey(0);
}