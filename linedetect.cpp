
#include "stdafx.h"
#include "sztx.h"

Image::Image(Mat &img)
{
	src = img;
	if (src.channels()==1)
		gray = src;
	else
		cvtColor(src, gray, CV_BGR2GRAY);
}

Image::~Image()
{
}

Mat Image::getCanny()
{
	double low_thresh, high_thresh;
	Mat tmp;
	high_thresh = threshold(gray, tmp, 0, 255, 
		THRESH_BINARY | THRESH_OTSU);
	low_thresh = high_thresh / 2;
	Canny(gray, canny, low_thresh, high_thresh);
	return canny;
}

int linedetect(string option, string img){
	Mat src = imread(img);
	if (src.empty())
		return -1;
	resizeToscreen(src, Size(1920 / 2, 1080 / 2));
	Mat kernal = getStructuringElement(MORPH_RECT,Size(5,5),Point(2,2));
	GaussianBlur(src, src, Size(5, 5), 2, 2);
	morphologyEx(src, src, MORPH_CLOSE, kernal);
	Image pic = Image(src);
	vector<Vec4i> lines;
	HoughLinesP(pic.getCanny(), lines, 1, CV_PI / 180, 80,
		min(src.rows, src.cols) / 2, min(src.rows, src.cols) / 5);
	for (int i = 0; i < lines.size(); i++)
	{
		Scalar color;
		color[0] = rand() % 255;
		color[1] = rand() % 255;
		color[2] = rand() % 255;
		line(src, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]),
			Scalar::all(255));
	}
	imshow("line detect", src);
	imshow("canny", pic.getCanny());

	//Mat gray;
	//cvtColor(src, gray, CV_BGR2GRAY);
	//imshow("gray", gray);

	waitKey();
	return 0;
}