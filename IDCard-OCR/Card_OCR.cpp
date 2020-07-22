//#include <iostream>
//#include <string>
//#include <stdlib.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/opencv.hpp>
//
//using namespace cv;
//using namespace std;
//
//Size Ksize(60, 90);
//void cv_show(string str, Mat Img);
//int getRCSum(Mat Img);
//void trackbar(int, void* userdata);
//int area = 50, proportion = 2;
//
//int main() {
//	Mat img = imread("Template.png");
//	Mat Img = imread("Template.png", 0);
//	Mat ref;
//	threshold(Img, ref, 10, 255, THRESH_BINARY_INV);
//	blur(ref, ref, Size(3, 3));
//
//	vector<vector<Point>> Contours;
//	vector<Vec4i> hierarchy;
//	Mat con;
//	ref.copyTo(con);
//	findContours(con, Contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));
//	drawContours(img, Contours, -1, (0, 0, 255), 3);
//
//	vector<Mat>Template;
//	for (int i = 0; i < Contours.size(); i++) {
//		Rect rect = boundingRect(Mat(Contours[i]));
//		Mat roi = Mat(ref, rect);
//		resize(roi, roi, Ksize);
//		Template.push_back(roi);
//		cv_show("hi", roi);
//	}
//	reverse(Template.begin(), Template.end());
//
//	/***********************************************/
//	Mat rectKernel = getStructuringElement(MORPH_RECT, Size(9, 3));
//	Mat sqKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
//	/*vector<Mat>Test[2];
//	for (int i = 0; i <= 5; i++) {
//		char str[64];
//		sprintf_s(str,"credit_card_0%d.png",i);
//		Mat t_img = imread(str[i]);
//		Mat t_Img = imread(str[i], 0);
//		Test[i][0].push_back(t_img);
//		Test[i][1].push_back(t_Img);
//	}
//	*/
//	Mat t_img = imread("credit_card_01.png");
//	Mat t_Img = imread("credit_card_01.png", 0);
//
//	resize(t_img, t_img, Size(500, 500));
//	resize(t_Img, t_Img, Size(500, 500));
//
//	morphologyEx(t_Img, t_Img, MORPH_TOPHAT, rectKernel);
//
//	Mat sobel;
//	Sobel(t_Img, sobel, CV_64F, 1, 0, -1);
//	convertScaleAbs(sobel, sobel);
//
//	Mat close1;
//	morphologyEx(sobel, close1, MORPH_CLOSE, sqKernel);
//
//	Mat t_ref;
//	adaptiveThreshold(close1, t_ref, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 3, 10);
//
//	Mat close2;
//	morphologyEx(t_ref, close2, MORPH_CLOSE, rectKernel);
//
//	vector<vector<Point>>t_Contours;
//	vector<Vec4i>t_hierarchy;
//	Mat t_con;
//	close2.copyTo(t_con);
//	findContours(t_con, t_Contours, t_hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(-1, -1));
//
//	vector<Rect>t_Rect;
//	for (int i = 0; i < t_Contours.size(); i++) {
//		Rect rect = boundingRect(Mat(t_Contours[i]));
//		double k = rect.width / rect.height;
//		int w = rect.width, h = rect.height;
//		if (k < 2.5) {
//			if (w > 85 && w < 160 && h>40 && h < 60) {
//				if (rect.y < t_img.rows * 2 / 3) {
//					t_Rect.push_back(rect);
//				}
//			}
//		}
//	}
//
//	for (int i = 0; i < t_Rect.size(); i++) {
//		for (int j = 0; j < t_Rect.size(); j++) {
//			if (i<j && t_Rect[i].x > t_Rect[j].x) {
//				Rect rect_ = t_Rect[i];
//				t_Rect[i] = t_Rect[j];
//				t_Rect[j] = rect_;
//			}
//		}
//	}
//
//	t_Rect[2].y += 17;
//	t_Rect[2].height -= 17;
//
//	t_Rect[0].x += 9;
//	t_Rect[0].width -= 22;
//
//	vector<vector<Mat>>N_TEST(4);
//	vector<vector<Rect>>N_Rect(4);
//	for (int i = 0; i < t_Rect.size(); i++) {
//		Mat Num = Mat(t_Img, t_Rect[i]);
//
//		resize(Num, Num, Size(300, 150));
//
//		Mat N_ref;
//
//		threshold(Num, N_ref, 11, 255, THRESH_BINARY);
//		blur(N_ref, N_ref, Size(3, 3));
//
//		vector<vector<Point>> N_Contours;
//		vector<Vec4i> N_hierarchy;
//		Mat N_con;
//		N_ref.copyTo(N_con);
//		findContours(N_con, N_Contours, N_hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//		for (int n = 0; n < N_Contours.size(); n++) {
//			Rect N_rect = boundingRect(Mat(N_Contours[n]));
//			N_Rect[i].push_back(N_rect);
//		}
//
//		for (int a = 0; a < N_Rect[i].size(); a++) {
//			for (int b = 0; b < N_Rect[i].size(); b++) {
//				if (a<b && N_Rect[i][a].x > N_Rect[i][b].x) {
//					Rect rect_ = N_Rect[i][a];
//					N_Rect[i][a] = N_Rect[i][b];
//					N_Rect[i][b] = rect_;
//				}
//			}
//		}
//
//		for (int c = 0; c < N_Rect[i].size(); c++) {
//			Mat dst = Mat(N_ref, N_Rect[i][c]);
//			resize(dst, dst, Ksize);
//			N_TEST[i].push_back(dst);
//		}
//	}
//	//开始配对；
//	vector<char> result;
//	for (int i = 0; i < t_Rect.size(); i++)
//	{
//		rectangle(t_img, t_Rect[i], Scalar(0, 0, 255), 2);
//
//		for (int b = 0; b < N_TEST[0].size(); b++) {
//			Mat subImg;
//			int sum[10] = { 0 };
//			int imin = 0;
//			int min_seq = 0;//记录最小的和对应的数字的编码
//
//			for (int j = 0; j < Template.size(); j++)
//			{
//				absdiff(N_TEST[i][b], Template[j], subImg);
//				sum[j] = getRCSum(subImg);
//				matchTemplate(N_TEST[i][b], Template[j], subImg, TM_SQDIFF_NORMED);
//
//				Point min_loc, max_loc;
//				double min_val, max_val;
//				minMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc, NULL);
//				cout << min_loc << " " << min_val;
//			}
//
//			int min = sum[0];
//			for (int m = 0; m < Template.size(); m++)
//			{
//				if (sum[m] < min)
//				{
//					imin = m;
//					min = sum[m];
//				}
//			}
//			result.push_back(imin + 48);
//		}
//		string str = "";
//
//		for (vector<char>::iterator iter = result.begin(); iter != result.end(); ++iter)
//		{
//			str += *iter;
//		}
//
//		Point dst(t_Rect[i].x - 3, t_Rect[i].y - 10);
//		cv::putText(t_img, str, dst, FONT_HERSHEY_SIMPLEX, 1.2, Scalar(2, 10, 255), 2);
//
//		vector<char>().swap(result);
//	}
//
//	namedWindow("result", 0);
//	cv_show("result", t_img);
//
//	/*namedWindow("lunkuo", WINDOW_AUTOSIZE);
//	createTrackbar("area:", "lunkuo", &area, 200, trackbar, &t_Contours);
//	createTrackbar("proportion:", "lunkuo", &proportion, 5, trackbar, &t_Contours);
//	waitKey(0);*/
//
//	return 0;
//}
//void trackbar(int, void* userdata) {
//	vector<vector<Point>>t_Contours = *(vector<vector<Point>>*)userdata;
//	Mat t_img = imread("credit_card_01.png");
//	resize(t_img, t_img, Size(500, 500));
//	for (int i = 0; i < t_Contours.size(); i++) {
//		Rect rect = boundingRect(Mat(t_Contours[i]));
//		double k = rect.width / rect.height;
//		int ar = rect.height + rect.width;
//
//		if (k < proportion - 0.5 && rect.width > 85 && rect.width < 160 && rect.height>40 && rect.height < 60) {
//			rectangle(t_img, rect.tl(), rect.br(), Scalar(0, 0, 0), 3);
//
//			cout << rect.height << endl;
//		}
//	}
//	cv_show("lunkuo", t_img);
//}
//void cv_show(string str, Mat Img)
//{
//	imshow(str, Img);
//	waitKey(0);
//	destroyAllWindows();
//}
//int getRCSum(Mat Img)//获取所有像素点和
//{
//	int RCSum = 0;
//	for (int i = 0; i < Img.rows; i++)
//	{
//		for (int j = 0; j < Img.cols; j++)
//		{
//			RCSum += Img.at <uchar>(i, j);
//		}
//	}
//	return RCSum;
//}