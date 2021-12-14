#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>                   
#include <math.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// �N��i�Ϲ���b�P�@����
void showImage(Mat src1, Mat src2, String s)
{
	// �إ߷s�Ϲ�
	int w = src1.cols + src2.cols;
	int h = src1.rows;
	Mat MergeImage = Mat::zeros(Size(w, h), src1.type());
	// �إ߹Ϲ�ROI
	Rect r1(0, 0, src1.cols, src1.rows);
	Rect r2(0, 0, src2.cols, src2.rows);
	r2.x = src1.cols;
	// �ƻs���e
	src1.copyTo(MergeImage(r1));
	src2.copyTo(MergeImage(r2));
	imshow(s, MergeImage);
}
// �e�����
Mat Draw(Mat &src)
{
	Mat hist;
	int size = 256;
	float range[] = { 0, 256 }; //�d�� 0-255
	const float* Range = { range };
	// �p��Ϲ��������
	calcHist(&src, 1, 0, Mat(), hist, 1, &size, &Range);
	// 0���¦� 255�զ�
	Mat final_pic(256, 256, CV_8U, Scalar(255));
	float maxValue = 0;
	// ��maxValue
	for (int i = 0; i < 256; i++) {
		if (hist.at<float>(i) > maxValue)
			maxValue = hist.at<float>(i);
	}
	for (int i = 0; i < 256; i++) {
		int realV = static_cast<int>(hist.at<float>(i) * 10 * 256 / maxValue);
		line(final_pic, Point(i, 255), Point(i, 255 - realV), Scalar(0));
	}
	return final_pic;
}

void Power_Law(Mat src, Mat dst, double Y);
int main() 
{
	/*  Cameraman.bmp  */
	double Y	= 0.5; // ��JGamma��
	string pic	= "Cameraman.bmp";
	Mat src = imread(pic, -1);
	Mat pl	= imread(pic, -1);
	// �������ഫ
	Power_Law(src, pl, Y);
	// ��ܵ��G
	showImage(src, Draw(src), "Cameraman_Original");		// ��ܭ��
	showImage(pl, Draw(pl), "Cameraman_Power_Law  Y=0.5");	// ���Power_Law���G

	/*  Peppers.bmp  */
	Y	= 3.5; // ��JGamma��
	pic = "Peppers.bmp";
	src = imread(pic, -1);
	pl	= imread(pic, -1);
	// �������ഫ
	Power_Law(src, pl, Y);
	// ��ܵ��G
	showImage(src, Draw(src), "Peppers_Original");			// ��ܭ��
	showImage(pl, Draw(pl), "Peppers_Power_Law  Y=3.5");	// ���Power_Law���G

	/*  Lena.bmp  */
	Y	= 0.4; // ��JGamma��
	pic = "Lena.bmp";
	src = imread(pic, -1);
	pl	= imread(pic, -1);
	// �������ഫ
	Power_Law(src, pl, Y);
	// ��ܵ��G
	showImage(src, Draw(src), "Lena_Original");			// ��ܭ��
	showImage(pl, Draw(pl), "Lena_Power_Law  Y=0.4");	// ���Power_Law���G

	waitKey(0);
	return 0;
}

void Power_Law(Mat src, Mat dst, double Y)
{
	//Ū��Mat
	uchar* readImage	= src.data;
	uchar* writeImage	= dst.data;
	double c; //�`�� �N�ഫ��v���վ�^0~255
	c = 255 / (pow(255, Y));
	for (int i = 0; i < src.total(); i++)
	{
		int r = readImage[i];
		int s = c * pow(r, Y);
		writeImage[i] = s;
	}
}