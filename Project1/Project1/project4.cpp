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
	//通过使用相对于图像大小的大内核尺寸模糊得到背景图
	blur(img, pattern, Size(img.cols / 3, img.cols / 3));
	return pattern;
}

Mat removeLight(Mat img, Mat pattern)
{
	Mat result;
	Mat img32, pattern32;
	img.convertTo(img32, CV_32F);
	pattern.convertTo(pattern32, CV_32F);
	//通过背景图像移除背景
	result = 1 - (pattern32 / img32);
	result = result * 255;
	result.convertTo(result, CV_8U);
	/*result = img-pattern;*/
	return result;
}

Mat preprocessImage(Mat input)
{
	//高斯降噪
	GaussianBlur(input, input, Size(5, 5), 0, 0);
	//获得背景
	Mat imgPattern = calculateLightPattern(input);
	//移除背景
	input = removeLight(input, imgPattern);
	//阈值化
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
	//通过连通组件算法分割
	Mat labels, stats, centroids;
	//connectedComponentsWithStats(输入数组 	图像 img，输出数组 	标签 labels，输出数组 	统计数据stats，输出数组 	质心centroids,整数 	连通性 = 8,整数 	ltype = CV_32S)
	int num_objects = connectedComponentsWithStats(img, labels, stats, centroids);
	//检查检测出的数目数量
	if (num_objects < 2) {
		cout << "No objects detected" << endl;
	}
	else {
		cout << "Number of objects detected: " << num_objects - 1 << endl;
	}
	//展示图像分割结果

	RNG randomNumGenerator(0xFFFFFFFF);
	Mat output = Mat::zeros(img.rows, img.cols, CV_8UC3);
	for (int i = 1; i < num_objects; i++)
	{
		Mat mask = labels == i;
		output.setTo(randomColor(randomNumGenerator), mask);
		//给目标赋上颜色以及面积标签
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

//提取特征向量
//std::vector< std::vector<float> > extractFeatures(cv::Mat img,std::vector<int>* left = NULL, std::vector<int>* top = NULL)
//{
//	std::vector< std::vector<float> > output;
//	std::vector<std::vector<cv::Point> > contours;
//	std::vector<cv::Vec4i> hierarchy;
//	//findContours会改变输入图像先备份
//	cv::Mat input = img.clone();
//	//检测对象
//	findContours(input, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
//	//检测结果检验
//	if (contours.size() == 0) {
//		return output;
//	}
//	cv::RNG rng(0xFFFFFFFF);
//	for (int i = 0; i < contours.size(); i++)
//	{
//		cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
//		//使用值1绘制形状，可以求和统计面积
//		drawContours(mask, contours, i, cv::Scalar(1), cv::FILLED, cv::LINE_8, hierarchy, 1);
//		cv::Scalar area_s = sum(mask);
//		float area = area_s[0];//得到第一个特征面积
//		//如果面积大于最小阈值
//		float MIN_AREA = 500;
//		if (area > MIN_AREA)
//		{
//			cv::RotatedRect r = minAreaRect(contours[i]);
//			float width = r.size.width;
//			float height = r.size.height;
//			//第二个特征纵横比
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
	//灰度
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	imgThr = preprocessImage(imgGray);
	//通过连通组件算法分割
	imgReslut = centroids(imgThr);
	//std::vector< std::vector<float> > feature;

	//feature = extractFeatures(imgThr);

	//cout << feature << endl;

	imshow("Img", imgGray);
	imshow("Img Thr", imgThr);
	imshow("Img Result", imgReslut);

	waitKey(0);
}