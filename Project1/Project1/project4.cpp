#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <random>

using namespace std;
using namespace cv;

Mat imgPattern, imgRemove, imgPre, imgGray, imgThr, imgReslut;
Mat calculateLightPattern(Mat img)
{
	Mat pattern;
	//ͨ��ʹ�������ͼ���С�Ĵ��ں˳ߴ�ģ���õ�����ͼ
	blur(img, pattern, Size(img.cols / 3, img.cols / 3));
	return pattern;
}

Mat removeLight(Mat img, Mat pattern)
{
	Mat result;
	Mat img32, pattern32;
	img.convertTo(img32, CV_32F);
	pattern.convertTo(pattern32, CV_32F);
	//ͨ������ͼ���Ƴ�����
	result = 1 - (pattern32 / img32);
	result = result * 255;
	result.convertTo(result, CV_8U);
	/*result = img-pattern;*/
	return result;
}

Mat preprocessImage(Mat input)
{
	//��˹����
	GaussianBlur(input, input, Size(5, 5), 0, 0);
	//��ñ���
	Mat imgPattern = calculateLightPattern(input);
	//�Ƴ�����
	input = removeLight(input, imgPattern);
	//��ֵ��
	Mat imageThr;
	threshold(input, imageThr, 50, 255, THRESH_BINARY);

	return imageThr;
}

static Scalar randomColor(RNG& rng)
{
	int icolor = (unsigned)rng;
	return Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

Mat centroids(Mat img)
{
	//ͨ����ͨ����㷨�ָ�
	Mat labels, stats, centroids;
	//connectedComponentsWithStats(�������� 	ͼ�� img��������� 	��ǩ labels��������� 	ͳ������stats��������� 	����centroids,���� 	��ͨ�� = 8,���� 	ltype = CV_32S)
	int num_objects = connectedComponentsWithStats(img, labels, stats, centroids);
	//����������Ŀ����
	if (num_objects < 2) {
		cout << "No objects detected" << endl;
	}
	else {
		cout << "Number of objects detected: " << num_objects - 1 << endl;
	}
	//չʾͼ��ָ���

	RNG randomNumGenerator(0xFFFFFFFF);
	Mat output = Mat::zeros(img.rows, img.cols, CV_8UC3);
	for (int i = 1; i < num_objects; i++)
	{
		Mat mask = labels == i;
		output.setTo(randomColor(randomNumGenerator), mask);
		//��Ŀ�긳����ɫ�Լ������ǩ
		stringstream ss;

		int area = stats.at<int>(i, CC_STAT_AREA);
		if (area > 500)
		{
			ss << "area: " << stats.at<int>(i, CC_STAT_AREA);
			//cout << labels << endl;
			cout << centroids.at<Point2d>(i) << endl;
			putText(output, ss.str(), centroids.at<Point2d>(i),FONT_HERSHEY_SIMPLEX,0.4,Scalar(255, 255, 255));
			cout << "222" << endl;
		}
	}
	return output;
}

//��ȡ��������
//std::vector< std::vector<float> > extractFeatures(cv::Mat img,std::vector<int>* left = NULL, std::vector<int>* top = NULL)
//{
//	std::vector< std::vector<float> > output;
//	std::vector<std::vector<cv::Point> > contours;
//	std::vector<cv::Vec4i> hierarchy;
//	//findContours��ı�����ͼ���ȱ���
//	cv::Mat input = img.clone();
//	//������
//	findContours(input, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
//	//���������
//	if (contours.size() == 0) {
//		return output;
//	}
//	cv::RNG rng(0xFFFFFFFF);
//	for (int i = 0; i < contours.size(); i++)
//	{
//		cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
//		//ʹ��ֵ1������״���������ͳ�����
//		drawContours(mask, contours, i, cv::Scalar(1), cv::FILLED, cv::LINE_8, hierarchy, 1);
//		cv::Scalar area_s = sum(mask);
//		float area = area_s[0];//�õ���һ���������
//		//������������С��ֵ
//		float MIN_AREA = 500;
//		if (area > MIN_AREA)
//		{
//			cv::RotatedRect r = minAreaRect(contours[i]);
//			float width = r.size.width;
//			float height = r.size.height;
//			//�ڶ��������ݺ��
//			float ar = (width < height) ? height / width : width / height;
//			std::vector<float> row;
//			row.push_back(area);
//			row.push_back(ar);
//			output.push_back(row);
//			if (left != NULL) {
//				left->push_back((int)r.center.x);
//			}
//			if (top != NULL) {
//				top->push_back((int)r.center.y);
//			}
//		}
//	}
//	return output;
//}

void main()
{
	string path = "Resources/1.jpg";
	//targetDectation(path);
	Mat img = imread(path);
	//�Ҷ�
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	imgThr = preprocessImage(imgGray);
	//ͨ����ͨ����㷨�ָ�
	imgReslut = centroids(imgThr);
	//std::vector< std::vector<float> > feature;

	//feature = extractFeatures(imgThr);

	//cout << feature << endl;

	imshow("Img", imgGray);
	imshow("Img Thr", imgThr);
	imshow("Img Result", imgReslut);

	waitKey(0);
}