#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <algorithm>

using namespace std;
using namespace cv;

//src:待处理图片
//dstImg:上下切割后图片
//psImg【6】: 左右切割后图片

Size dsize(1000, 1000);
const int p = 4;//比赛是要识别的数字数目。
const int num = rand() % 100;

//函数声明
int getColSum(Mat src, int col);
int getRowSum(Mat src, int row);
Rect cutTop(Mat src);
int cutHen(Mat dstImg);
int getRCSum(Mat Img);

int Normal_OCR() //数字识别
{
	//Mat frame;
	//int i = 0;
	////Mat excemple = imread("F:\\opencvRM\\picture\\a.jpg");//模板原图123456*3
	//VideoCapture capture(0);//应使用usb的彩色摄像头
	//string name;
	//namedWindow("图片捕捉", WINDOW_NORMAL);
	//while (capture.isOpened())
	//{
	//	capture >> frame;
	//	if (i <= 6)
	//	{
	//		name = "F:\\opencvRM\\picture\\" + to_string(i) + ".jpg";
	//		imwrite(name, frame);
	//		cout << name << endl;
	//		i++;
	//	}
	//	else break;
	//	imshow("图片捕捉", frame);
	//}
	Mat src = imread("F:\\opencvRM\\picture\\c1.jpg", 0);//读取图片//记得比赛时修改
	threshold(src, src, 100, 255, 1);//二值化
	waitKey(1000);

	//全面切割
	Rect rect = cutTop(src);
	Mat dstImg = src(rect).clone();
	//imwrite("F:\\opencvRM\\picture\\灰度二值化预处理.jpg", dstImg);

	char store[64] = { 0 };
	sprintf_s(store, "F:\\opencvRM\\RMpicture\\%d.jpg", num);
	imwrite(store, dstImg);

	cutHen(dstImg);

	//加载模板组
	vector<Mat> myTemplate;
	for (int i = 0; i < 18; i++)
	{
		char name[64];//需要预先分配缓冲区
		sprintf_s(name, "F:\\opencvRM\\exemple\\%d.jpg", i);
		Mat temp = imread(name, 0);
		threshold(temp, temp, 100, 255, 1);//二值化
		resize(temp, temp, dsize, 0, 0, INTER_CUBIC);
		myTemplate.push_back(temp);
	}
	//加载处理组
	vector<Mat> myROI;
	for (int i = 0; i < p; i++)
	{
		char feyker[64];
		sprintf_s(feyker, "F:\\opencvRM\\picture\\%d.jpg", i);
		Mat ROI = imread(feyker, 0);
		resize(ROI, ROI, dsize, 0, 0, INTER_CUBIC);
		myROI.push_back(ROI);
	}
	//进行数字识别
	//顺序存放识别结果
	vector<int> seq;
	for (int i = 0; i < p; i++)
	{
		Mat subImage;
		int sum[18];
		int imin;
		int min_seq = 0;//记录最小的和对应的数字的编码
		for (int j = 0; j < 18; j++)
		{
			//计算两个图片的差值
			absdiff(myROI[i], myTemplate[j], subImage);
			sum[j] = getRCSum(subImage);
		}
		//冒泡排序
		int min = sum[0];
		for (int j = 0; j < 18; j++)
		{
			if (sum[j] < min)
			{
				imin = j;
				min = sum[j];
			}
		}
		min_seq = imin;
		seq.push_back(min_seq);
	}

	//保存结果
	ofstream fout;
	fout.open("NumberOCR_result.txt", ios::app);
	for (int i = 0; i < p; i++)
	{
		fout << "对应数字模板的结果：" << seq[i] << endl;
	}
	fout << "******************************************" << endl;
	fout.close();
	//输出结果
	std::cout << "数字识别结果为:" << endl;
	for (int i = 0; i < seq.size(); i++)
	{
		switch (seq[i])
		{
		case 0:
		case 6:
		case 12:
			std::cout << "1" << endl;
			break;
		case 1:
		case 7:
		case 13:
			std::cout << "2" << endl;
			break;
		case 2:
		case 8:
		case 14:
			std::cout << "3" << endl;
			break;
		case 3:
		case 9:
		case 15:
			std::cout << "4" << endl;
			break;
		case 4:
		case 10:
		case 16:
			std::cout << "5" << endl;
			break;
		case 5:
		case 11:
		case 17:
			std::cout << "6" << endl;
			break;
		default:
			break;
		}
	}
	std::cout << endl;
	std::cout << "识别结束";
	//无法修复的Bug：识别效果不是那么准确，“0”中的1经常被识别为“6”中的1
	return 0;
}

int getColSum(Mat src, int col)//统计所有列像素的总和
{
	int sum = 0;
	for (int i = 0; i < src.rows; i++)
	{
		sum += src.at <uchar>(i, col);//表示的是i行 col列 的这个像素
	}
	return sum;
}

int getRowSum(Mat src, int row)//统计所有行像素的总和
{
	int sum = 0;
	for (int i = 0; i < src.cols; i++)
	{
		sum += src.at <uchar>(row, i);
	}
	return sum;
}

int getRCSum(Mat Img)//获取所有像素点和
{
	int RCSum = 0;
	for (int i = 0; i < Img.rows; i++)
	{
		for (int j = 0; j < Img.cols; j++)
		{
			RCSum += Img.at <uchar>(i, j);
		}
	}
	return RCSum;
}

Rect cutTop(Mat src)//上下切割
{
	int top = 0, bottom = src.rows;
	int i;
	for (i = 0; i < src.rows; i++)
	{
		int RowSum = getRowSum(src, i);//统计所有行像素的总和
		if (RowSum > 0)//扫描直到行像素的总和大于0时，记下当前位置top
		{
			top = i;
			break;
		}
	}
	for (; i < src.rows; i++)
	{
		int colValue = getRowSum(src, i);//统计所有行像素的总和
		if (colValue == 0 && i > src.rows * (2 / 3))//继续扫描直到行像素的总和等于0时，记下当前位置bottom
		{
			bottom = i;
			break;
		}
	}
	int height = bottom - top;
	Rect rect(0, top, src.cols, height);
	return rect;
}

int cutHen(Mat dstImg)//左右切割
{
	int left[64] = { 0 };
	int right[64] = { 0 };
	int width[64] = { 0 };
	Mat psImg[64];
	Rect rect;
	int i;
	for (i = 0; i < dstImg.cols; i++)
	{
		int ColSum = getColSum(dstImg, i);//统计所有列像素的总和

		if (ColSum > 0)//扫描直到列像素的总和大于0时，记下当前位置left
		{
			left[0] = i;
			break;
		}
	}
	//继续扫描
	for (; i < dstImg.cols; i++)
	{
		int colValue = getColSum(dstImg, i);//统计所有列像素的总和
		if (colValue == 0)//继续扫描直到列像素的总和等于0时，记下当前位置right
		{
			right[0] = i;
			break;
		}
	}
	for (int t = 1; t < 6; t++)
	{
		int i;
		for (i = 0; i < dstImg.cols; i++)
		{
			int ColSum = getColSum(dstImg, i);//统计所有列像素的总和

			if (ColSum > 0 && i > right[t - 1])//扫描直到列像素的总和大于0时，记下当前位置left
			{
				left[t] = i;
				break;
			}
		}
		//继续扫描
		for (; i < dstImg.cols; i++)
		{
			int colValue = getColSum(dstImg, i);//统计所有列像素的总和
			if (colValue == 0)//继续扫描直到列像素的总和等于0时，记下当前位置right
			{
				right[t] = i;
				break;
			}
		}
	}
	for (int t = 0; t < 6; t++)
	{
		width[t] = right[t] - left[t];
		Rect rect(left[t], 0, width[t], dstImg.rows);
		psImg[t] = dstImg(rect).clone();
		char ad[64] = { 0 };
		sprintf_s(ad, "F:\\opencvRM\\picture\\%d.jpg", t);
		imwrite(ad, psImg[t]);
	}
	return 0;
}