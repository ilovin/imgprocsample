#include "stdafx.h"
#include "sztx.h"

using namespace std;
using namespace cv;

void peper_salt(Mat&src, double persentage);
Mat arith_filter(Mat src,int kernal_size);
Mat geometry_filter(Mat src, int kernal_size);

int _smooth(string img, string type)
{
	int w_pic, h_pic;
	Mat src = imread(img);
	Mat final_res;
	w_pic = src.cols;
	h_pic = src.rows;
	while (src.rows>1080 / 3 || src.cols>1920 / 3)
	{
		w_pic = w_pic * 2 / 3;
		h_pic = h_pic * 2 / 3;
		resize(src, src, Size(w_pic, h_pic));
	}
	peper_salt(src,0.1);
	src.convertTo(src, CV_32FC3);
	src = src / 255;
	//imshow("src", src);
	Mat dst;
	if (type=="-arithmetic"||type=="-a")
	{
		final_res = Mat(src.rows, src.cols * 2, CV_32FC3);
		drawtheblock(src, final_res, Point(0, 0), "peper & salt");
		int tmp = 3;
		while (waitKey(500)!=27)
		{
			drawtheblock(arith_filter(src, tmp), final_res, Point(src.cols, 0), "after filter");
			imshow("display", final_res);
			tmp += 2;
			if (tmp>src.rows/4||tmp>src.cols/4)
				break;
		}
	}
	if (type=="-geometry"||type=="-g")
	{
		final_res = Mat(src.rows, src.cols * 2, CV_32FC3);
		drawtheblock(src, final_res, Point(0, 0), "peper & salt");
		int tmp = 3;
		drawtheblock(geometry_filter(src, tmp), final_res, Point(src.cols, 0), "after filter");
		imshow("display", final_res);

	}
	waitKey(0);

	return 0;
}

Mat arith_filter(Mat src,int kernal_size)
{
	Mat kernel = Mat::ones(kernal_size, kernal_size, CV_32F) / (kernal_size*kernal_size);
	Mat dst;
	filter2D(src, dst, -1, kernel, Point(-1, -1));
	return dst;
}
Mat geometry_1channal(Mat src, int kernal_size)
{
	Mat mid_mat,dst;
	dst = Mat(src.size(), src.type());
	int tmp = (kernal_size - 1) / 2;
	copyMakeBorder(src, mid_mat, tmp, tmp, tmp, tmp, BORDER_REFLECT_101);
	for (int i = tmp; i < src.rows+tmp; i++)
	{
		for (int j = tmp; j < src.cols+tmp; j++)
		{
			float sum = 1;
			for (int k = -tmp; k <= tmp; k++)
			{
				for (int l = -tmp; l <= tmp; l++)
				{
					sum *= mid_mat.at<float>(k+i, l+j);
				}
			}
			sum = pow(sum, 1.0 / (kernal_size*kernal_size));
			dst.at<float>(i-tmp, j-tmp) = sum;
		}
	}
	return dst;
	
}
Mat geometry_filter(Mat src, int kernal_size)
{
	int tmp = (kernal_size - 1) / 2;
	Mat mid_mat,dst;
	Mat bgr[3];
	split(src, bgr);
	bgr[0]=geometry_1channal(bgr[0], kernal_size);
	bgr[1]=geometry_1channal(bgr[1], kernal_size);
	bgr[2]=geometry_1channal(bgr[2], kernal_size);
	merge(bgr,3, dst);
	return dst;

}
void peper_salt(Mat&src, double persentage)
{
	int n = cvRound(persentage*src.rows*src.cols);
	int x, y,seasonor,chns;
	chns = src.channels();
	for (int k = 0; k < n; k++)
	{
		x = rand() % src.rows;
		y = rand() % src.cols;
		if (rand() % 2)
			seasonor = 0;
		else
			seasonor = 255;
		if (chns==1)
		{
			src.at<uchar>(x, y) = seasonor;
		}
		else
		{
			for (int i = 0; i < chns; i++)
			{
				src.at<Vec3b>(x, y)[0] = seasonor;
				src.at<Vec3b>(x, y)[2] = seasonor;
				src.at<Vec3b>(x, y)[1] = seasonor;
			}

		} 
	}

}