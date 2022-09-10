#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/objdetect.hpp>

using namespace std;
using namespace cv;


// video
void main() {
	VideoCapture cap(0);
	Mat img;
	//string path = "Resources/test.png";
	//Mat img = imread(path);
	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");
	if (plateCascade.empty()) { cout << "xml !!!" << endl; }

	vector<Rect> plates;

	while (true)
	{
		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.1, 10);
		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = img(plates[i]);
			imwrite("Resources/Plates/" + to_string(i) + ".jpg", imgCrop);
			//imshow(to_string(i), imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(0, 255, 0), 5);
		}

		imshow("Img", img);
		waitKey(1);
	}

}


