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
	CascadeClassifier faceCascade;
	faceCascade.load("Resources/haarcascade_frontalface_default.xml");
	if (faceCascade.empty()){cout << "xml !!!" << endl;}

	vector<Rect> faces;

	while (true) 
	{
		cap.read(img);
		faceCascade.detectMultiScale(img, faces, 1.1, 10);
		for (int i = 0; i < faces.size(); i++)
		{
			rectangle(img, faces[i].tl(), faces[i].br(), Scalar(0, 255, 0), 5);
		}

		imshow("Img", img);
		waitKey(1);
	}

}