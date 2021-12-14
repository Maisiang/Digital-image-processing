#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>                                    
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

void Histogram_Equalization(Mat src, Mat dst);

int main() {
	/*  Peppers.bmp  */
	string pic = "Peppers.bmp";
	Mat src = imread(pic, 0);
	Mat HE = imread(pic, 0);

	Histogram_Equalization(src, HE); // ������ϵ���
	showImage(src, Draw(src), "Peppers_Original");	// ��ܭ��
	showImage(HE, Draw(HE), "Peppers_Histogram_Equalization"); // Global����ϵ��Ƶ��G

	/*  Lena.bmp  */
	pic = "Lena.bmp";
	src = imread(pic, 0);
	HE = imread(pic, 0);

	Histogram_Equalization(src, HE); // ������ϵ���
	showImage(src, Draw(src), "Lena_Original"); // ��ܭ��
	showImage(HE, Draw(HE), "Lena_Histogram_Equalization"); // Global����ϵ��Ƶ��G

	/*  Cameraman.bmp  */
	pic = "Cameraman.bmp";
	src = imread(pic, 0);
	HE = imread(pic, 0);
	Histogram_Equalization(src, HE); // ������ϵ���
	showImage(src, Draw(src), "Cameraman_Original"); // ��ܭ��
	showImage(HE, Draw(HE), "Cameraman_Histogram_Equalization"); // Global����ϵ��Ƶ��G

	waitKey(0);
	return 0;
}
void  Histogram_Equalization(Mat src, Mat dst)
{
	int MN = 65536;
	int L = 256;
	int n[256]; // �Ƕ��ȥX�{����
	double p[256]; // �X�{���v
	double s[256]; // ���ƭ�

	// ��l�ư}�C
	for (int i = 0; i < 256; i++)
	{
		n[i] = 0;
	}
	// Ū���Ƕ��ȼƶq n
	uchar* readImage = src.data;
	for (int i = 0; i < src.total(); i++)
	{
		//printf("%u ",p[i] );
		int num = readImage[i];
		n[num]++;
	}
	// �� p
	for (int i = 0; i < 256; i++)
	{
		p[i] = double(n[i]) / double(MN);
	}
	// �� s
	int L1 = L - 1;
	s[0] = p[0] * (L - 1);
	for (int i = 1; i < 256; i++)
	{
		s[i] = p[i] * double(L1) + s[i - 1];
	}
	// �|�ˤ��J
	int result[256];
	char tmp[14];
	for (int i = 0; i < 256; i++)
	{
		result[i] = 0;
		sprintf(tmp, "%.0lf", s[i]);
		result[i] = atoi(tmp);
	}
	// ���Ƶ��G�s�^dst
	uchar* writeImage = dst.data;
	for (int i = 0; i < dst.total(); i++)
	{
		int num = writeImage[i];
		writeImage[i] = result[num];
	}
}