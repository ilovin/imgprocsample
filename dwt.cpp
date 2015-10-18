
#include "stdafx.h"

using namespace std;
using namespace cv;

void border_to_even(Mat& src);
Mat h_trans(Mat& src);
Mat v_trans(Mat& src);
Mat dwt_one_time(Mat& src);

int dwt(string img, int time)
{
	Mat src = imread(img,CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty())
		return -1;

	int w_pic = src.cols, h_pic = src.rows;
	while (src.rows>1080/2||src.cols>1920/2)
	{
		w_pic /= 2;
		h_pic /= 2;
		resize(src, src, Size(w_pic, h_pic));
	}
	border_to_even(src);
	//src.convertTo(src, CV_32F);
	//normalize(src, src, 0, 1, NORM_MINMAX);
	//imshow("src", src);
	//Mat dst = h_trans(src);
	//Mat dst = v_trans(src);
	Mat dst = dwt_one_time(src);
	imshow("tmp", src);
	Mat imgroi = dst(Rect(0, 0, dst.cols / 2, dst.rows / 2));
	for (int i = 0; i < time-1; i++)
	{
		dwt_one_time(imgroi);
		//imgroi.copyTo(dst);
		//imgroi = imgroi(Rect(0, 0, imgroi.cols / 2, imgroi.rows / 2));
		//dwt_one_time(src);
	}
	imshow("dst", dst);
	while (waitKey(30)!=27)
	{
		;
	}
	return 0;
}

void border_to_even(Mat& src)
{
	if (src.rows % 2 == 1)
		copyMakeBorder(src, src, 0, 1, 0, 0, BORDER_REFLECT_101);
	if (src.cols % 2 == 1)
		copyMakeBorder(src, src, 0, 0, 0, 1, BORDER_REFLECT_101);
}

Mat dwt_one_time(Mat& src)
{
	Mat dst = v_trans(src);
	dst = h_trans(dst);
	dst.copyTo(src);
	return dst;
}

Mat h_trans(Mat& src)
{
	Mat dst(src.size(), src.type());
	int half_rows = src.rows / 2;
	for (int i = 0; i < half_rows; i++)
	{
		src.row(2 * i).copyTo(dst.row(i));
		src.row(2 * i + 1).copyTo(dst.row(half_rows + i));
	}
	for (int i = 0; i < half_rows; i++)
	{
		dst.row(half_rows + i) -= dst.row(i);
	}
	Mat imgroi = dst(Rect(0, half_rows, src.cols, half_rows));
	normalize(imgroi, imgroi, 0, 255, NORM_MINMAX);
	dst.copyTo(src);
	return dst;
}

Mat v_trans(Mat& src)
{
	Mat dst(src.size(), src.type());
	int half_cols = src.cols / 2;
	for (int i = 0; i < half_cols; i++)
	{
		src.col(2 * i).copyTo(dst.col(i));
		src.col(2 * i + 1).copyTo(dst.col(half_cols + i));
	}
	for (int i = 0; i < half_cols; i++)
	{
		dst.col(half_cols + i) -= dst.col(i);
	}
	Mat	imgroi = dst(Rect(half_cols, 0, half_cols, src.rows));
	normalize(imgroi, imgroi, 0, 255, NORM_MINMAX);
	dst.copyTo(src);
	return dst;
}









