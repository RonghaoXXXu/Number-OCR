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

//src:������ͼƬ
//dstImg:�����и��ͼƬ
//psImg��6��: �����и��ͼƬ

Size dsize(1000, 1000);
const int p = 4;//������Ҫʶ���������Ŀ��
const int num = rand() % 100;

//��������
int getColSum(Mat src, int col);
int getRowSum(Mat src, int row);
Rect cutTop(Mat src);
int cutHen(Mat dstImg);
int getRCSum(Mat Img);

int Normal_OCR() //����ʶ��
{
	//Mat frame;
	//int i = 0;
	////Mat excemple = imread("F:\\opencvRM\\picture\\a.jpg");//ģ��ԭͼ123456*3
	//VideoCapture capture(0);//Ӧʹ��usb�Ĳ�ɫ����ͷ
	//string name;
	//namedWindow("ͼƬ��׽", WINDOW_NORMAL);
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
	//	imshow("ͼƬ��׽", frame);
	//}
	Mat src = imread("F:\\opencvRM\\picture\\c1.jpg", 0);//��ȡͼƬ//�ǵñ���ʱ�޸�
	threshold(src, src, 100, 255, 1);//��ֵ��
	waitKey(1000);

	//ȫ���и�
	Rect rect = cutTop(src);
	Mat dstImg = src(rect).clone();
	//imwrite("F:\\opencvRM\\picture\\�Ҷȶ�ֵ��Ԥ����.jpg", dstImg);

	char store[64] = { 0 };
	sprintf_s(store, "F:\\opencvRM\\RMpicture\\%d.jpg", num);
	imwrite(store, dstImg);

	cutHen(dstImg);

	//����ģ����
	vector<Mat> myTemplate;
	for (int i = 0; i < 18; i++)
	{
		char name[64];//��ҪԤ�ȷ��仺����
		sprintf_s(name, "F:\\opencvRM\\exemple\\%d.jpg", i);
		Mat temp = imread(name, 0);
		threshold(temp, temp, 100, 255, 1);//��ֵ��
		resize(temp, temp, dsize, 0, 0, INTER_CUBIC);
		myTemplate.push_back(temp);
	}
	//���ش�����
	vector<Mat> myROI;
	for (int i = 0; i < p; i++)
	{
		char feyker[64];
		sprintf_s(feyker, "F:\\opencvRM\\picture\\%d.jpg", i);
		Mat ROI = imread(feyker, 0);
		resize(ROI, ROI, dsize, 0, 0, INTER_CUBIC);
		myROI.push_back(ROI);
	}
	//��������ʶ��
	//˳����ʶ����
	vector<int> seq;
	for (int i = 0; i < p; i++)
	{
		Mat subImage;
		int sum[18];
		int imin;
		int min_seq = 0;//��¼��С�ĺͶ�Ӧ�����ֵı���
		for (int j = 0; j < 18; j++)
		{
			//��������ͼƬ�Ĳ�ֵ
			absdiff(myROI[i], myTemplate[j], subImage);
			sum[j] = getRCSum(subImage);
		}
		//ð������
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

	//������
	ofstream fout;
	fout.open("NumberOCR_result.txt", ios::app);
	for (int i = 0; i < p; i++)
	{
		fout << "��Ӧ����ģ��Ľ����" << seq[i] << endl;
	}
	fout << "******************************************" << endl;
	fout.close();
	//������
	std::cout << "����ʶ����Ϊ:" << endl;
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
	std::cout << "ʶ�����";
	//�޷��޸���Bug��ʶ��Ч��������ô׼ȷ����0���е�1������ʶ��Ϊ��6���е�1
	return 0;
}

int getColSum(Mat src, int col)//ͳ�����������ص��ܺ�
{
	int sum = 0;
	for (int i = 0; i < src.rows; i++)
	{
		sum += src.at <uchar>(i, col);//��ʾ����i�� col�� ���������
	}
	return sum;
}

int getRowSum(Mat src, int row)//ͳ�����������ص��ܺ�
{
	int sum = 0;
	for (int i = 0; i < src.cols; i++)
	{
		sum += src.at <uchar>(row, i);
	}
	return sum;
}

int getRCSum(Mat Img)//��ȡ�������ص��
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

Rect cutTop(Mat src)//�����и�
{
	int top = 0, bottom = src.rows;
	int i;
	for (i = 0; i < src.rows; i++)
	{
		int RowSum = getRowSum(src, i);//ͳ�����������ص��ܺ�
		if (RowSum > 0)//ɨ��ֱ�������ص��ܺʹ���0ʱ�����µ�ǰλ��top
		{
			top = i;
			break;
		}
	}
	for (; i < src.rows; i++)
	{
		int colValue = getRowSum(src, i);//ͳ�����������ص��ܺ�
		if (colValue == 0 && i > src.rows * (2 / 3))//����ɨ��ֱ�������ص��ܺ͵���0ʱ�����µ�ǰλ��bottom
		{
			bottom = i;
			break;
		}
	}
	int height = bottom - top;
	Rect rect(0, top, src.cols, height);
	return rect;
}

int cutHen(Mat dstImg)//�����и�
{
	int left[64] = { 0 };
	int right[64] = { 0 };
	int width[64] = { 0 };
	Mat psImg[64];
	Rect rect;
	int i;
	for (i = 0; i < dstImg.cols; i++)
	{
		int ColSum = getColSum(dstImg, i);//ͳ�����������ص��ܺ�

		if (ColSum > 0)//ɨ��ֱ�������ص��ܺʹ���0ʱ�����µ�ǰλ��left
		{
			left[0] = i;
			break;
		}
	}
	//����ɨ��
	for (; i < dstImg.cols; i++)
	{
		int colValue = getColSum(dstImg, i);//ͳ�����������ص��ܺ�
		if (colValue == 0)//����ɨ��ֱ�������ص��ܺ͵���0ʱ�����µ�ǰλ��right
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
			int ColSum = getColSum(dstImg, i);//ͳ�����������ص��ܺ�

			if (ColSum > 0 && i > right[t - 1])//ɨ��ֱ�������ص��ܺʹ���0ʱ�����µ�ǰλ��left
			{
				left[t] = i;
				break;
			}
		}
		//����ɨ��
		for (; i < dstImg.cols; i++)
		{
			int colValue = getColSum(dstImg, i);//ͳ�����������ص��ܺ�
			if (colValue == 0)//����ɨ��ֱ�������ص��ܺ͵���0ʱ�����µ�ǰλ��right
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