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
Mat Draw(Mat &src) {
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

int mask[3][3]	= { 0, -1, 0, -1, 4, -1, 0, -1, 0 };	//  Laplacian mask �|�F��
int mask2[3][3] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 };		//  Laplacian mask �K�F��

void Laplacian(Mat src, Mat dst);
int main() {
	/*  Cameraman.bmp  */
	string pic	= "Cameraman.bmp";
	Mat src		= imread(pic, 0);
	Mat dst		= imread(pic, 0);
	// ���Դ��Դ��U��
	Laplacian(src, dst);
	// ��ܵ��G
	showImage(src, Draw(src), "Cameraman_Original");	// ��ܭ��
	showImage(dst, Draw(dst), "Cameraman_Laplacian");	// ��ܾU�Ƶ��G

	/*  Peppers.bmp  */
	pic = "Peppers.bmp";
	src = imread(pic, 0);
	dst = imread(pic, 0);
	// ���Դ��Դ��U��
	Laplacian(src, dst);
	// ��ܵ��G
	showImage(src, Draw(src), "Peppers_Original");	// ��ܭ��
	showImage(dst, Draw(dst), "Peppers_Laplacian");	// ��ܾU�Ƶ��G

	/*  Lena.bmp  */
	pic = "Lena.bmp";
	src = imread(pic, 0);
	dst = imread(pic, 0);
	// ���Դ��Դ��U��
	Laplacian(src, dst);
	// ��ܵ��G
	showImage(src, Draw(src), "Lena_Original");	// ��ܭ��
	showImage(dst, Draw(dst), "Lena_Laplacian");	// ��ܾU�Ƶ��G

	waitKey(0);
	return 0;
}

void Laplacian(Mat src, Mat dst)
{
	int c = 1;
	for (int i = 1; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j++)
		{
			dst.at<uchar>(i, j) = saturate_cast<uchar>(mask[1][1] * src.at<uchar>(i, j) +
				(mask[0][0] * src.at<uchar>(i - 1, j - 1) + mask[1][0] * src.at<uchar>(i, j - 1) + mask[2][0] * src.at<uchar>(i + 1, j - 1) +
					mask[0][1] * src.at<uchar>(i - 1, j) + mask[2][1] * src.at<uchar>(i + 1, j) +
					mask[0][2] * src.at<uchar>(i - 1, j + 1) + mask[1][2] * src.at<uchar>(i, j + 1) + mask[2][2] * src.at<uchar>(i + 1, j + 1)));
			//�N��X�Ӫ��Դ��Դ��P�쥻�Ȭۥ[�s�^�h
			dst.at<uchar>(i, j) = saturate_cast<uchar>(src.at<uchar>(i, j) + c * dst.at<uchar>(i, j));
		}
	}
}