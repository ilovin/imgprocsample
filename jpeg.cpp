
#include "stdafx.h"
#include "sztx.h"

static float data[8][8] = {
	{ 16, 11, 10, 16, 24, 40, 51, 61 },
	{ 12, 12, 14, 19, 26, 58, 60, 55 },
	{ 14, 13, 16, 24, 40, 57, 69, 56 },
	{ 14, 17, 22, 29, 51, 87, 80, 62 },
	{ 18, 22, 37, 56, 68, 109, 103, 77 },
	{ 24, 35, 55, 64, 81, 104, 113, 92 },
	{ 49, 64, 78, 87, 103, 121, 120, 101 },
	{ 72, 92, 95, 98, 112, 100, 103, 99 }
};

int jpeg(string option, string img){
	Mat src = imread(img);
	if (src.empty()) return -1;
	//resizeToscreen(src, Size(1920 / 3, 1080 / 3));
	Mat deal = src.clone();
	copyMakeBorder(deal, deal, 0, deal.rows % 8, 0, deal.cols % 8, BORDER_REFLECT101);
	imshow("src", deal);
	deal.convertTo(deal, CV_BGR2YUV);
	deal.convertTo(deal, CV_32FC3);
	//deal = (Mat_<float>&)deal;
	cout << deal.at<Vec3f>(0, 0);
	deal -= Scalar::all(128);
	vector<Mat>planes;
	split(deal, planes);
	//comprass
	for (int i = 1; i < (int)planes.size(); i++)
	{
		resize(planes[i], planes[i], Size(deal.cols / 2, deal.rows / 2));
		resize(planes[i], planes[i], Size(deal.cols, deal.rows));
	}
	Mat quant = Mat(Size(8, 8), CV_32F, &data);
	for (int i = 0; i < deal.rows; i+=8)
	{
		for (int  j = 0; j < deal.cols; j+=8)
		{
			for (int k = 1; k < (int)planes.size(); k++)
			{
				Mat imgroi = planes[k](Rect(j,i,8,8));
				dct(imgroi, imgroi);
				divide(imgroi, quant, imgroi);
			}
		}
	}
	for (int i = 0; i < deal.rows; i+=8)
	{
		for (int  j = 0; j < deal.cols; j+=8)
		{
			for (int k = 1; k < (int)planes.size(); k++)
			{
				Mat imgroi = planes[k](Rect(j, i, 8, 8));
				multiply(imgroi, quant, imgroi);
				idct(imgroi, imgroi);
			}
		}
	}
	merge(planes, deal);
	deal += Scalar::all(128);
	deal.convertTo(deal, CV_YUV2RGB);
	//deal /= 255;
	deal.convertTo(deal, CV_8UC3);
	//deal = (Mat_<uchar>&)deal;
	Mat dst = deal(Rect(0, 0, src.cols, src.rows));
	Mat diff = src - dst;
	diff *= 16;
	imshow("diff", diff);
	//imshow("dst", dst);
	vector<int>params;
	params.push_back(IMWRITE_JPEG_QUALITY);
	params.push_back(60);
	imwrite("dst.jpg", dst,params);

	waitKey(0);

	return 0;
}