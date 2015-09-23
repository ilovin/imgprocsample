// sztx.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;
using namespace cv;

void diffcopy(Mat &src, Mat &dst);
void rgb2cmyk(Mat &src, vector<Mat> &cmyk);
void rgb2cmyk2(Mat &src, Mat &cmyk);
void drawtheblock(Mat &src,Mat &dst, Point anchor, String str);
Mat histgram(Mat &src);

int w_pic = int(1920 /6), h_pic = int(1080 / 6);

int _tmain(int argc, _TCHAR* argv[])
{
	Mat src = imread(argv[1]);
	if (src.empty())
	{
		cout << "cannot read the file" << endl;
		return -1;
	}
	resize(src, src, Size(w_pic, h_pic));
	imwrite("dst.bmp", src);
	//cout << src.size() << endl;
	//imshow("src", src);
	Mat gray,binary,rgb[3],hsv;
	cvtColor(src, gray, CV_BGR2GRAY);
	cvtColor(src, hsv, CV_BGR2HSV);
	threshold(gray, binary,0, 255,THRESH_BINARY|THRESH_OTSU);
	split(src, rgb);
	Mat dst(h_pic * 3, w_pic * 3,CV_8UC3);
	Mat cmyk;
	Mat eqh;
	equalizeHist(gray, eqh);

	//paint the pic
	drawtheblock(src, dst, Point(0, 0), "org");
	drawtheblock(gray, dst, Point(w_pic, 0), "gray");
	drawtheblock(binary, dst, Point(w_pic * 2, 0), "binary");
	drawtheblock(rgb[2], dst, Point(0, h_pic), "r");
	drawtheblock(rgb[1], dst, Point(w_pic, h_pic), "g");
	drawtheblock(rgb[0], dst, Point(w_pic * 2, h_pic), "b");
	drawtheblock(eqh, dst, Point(0, h_pic * 2), "equalizehist");
	drawtheblock(histgram(gray), dst, Point(w_pic, h_pic*2), "before");
	drawtheblock(histgram(eqh), dst, Point(w_pic * 2, h_pic * 2), "after");

	//imwrite("dst.png", dst);
	//Mat tmp = imread("dst.png");
	//namedWindow("tmp", WINDOW_FULLSCREEN);
	//imshow("tmp", tmp);
	//gr*/
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);
	waitKey(0);
	return 0;
}


void diffcopy(Mat &src, Mat &dst)
{
	Mat tmp;
	if (src.type() != dst.type())
	{
		cvtColor(src, tmp, CV_GRAY2BGR);
		tmp.copyTo(dst);
	}
	else{
		src.copyTo(dst);
	}
}

void rgb2cmyk(Mat &src, vector<Mat> &cmyk)
{
	for (int i = 0; i < 4; i++)
	{
		cmyk.push_back(Mat(src.size(), CV_8UC1));
	}
	Mat rgb[3];
	split(src, rgb);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			float r = (float)rgb[2].at<uchar>(i, j) / 255;
			float g = (float)rgb[1].at<uchar>(i, j) / 255;
			float b = (float)rgb[0].at<uchar>(i, j) / 255; 
			float k = min(min(r, g), b);
			cmyk[0].at<uchar>(i, j) = int((1 - r - k) / (1 - k) * 255);
			cmyk[1].at<uchar>(i, j) = int((1 - g - k) / (1 - k) * 255);
			cmyk[2].at<uchar>(i, j) = int((1 - b - k) / (1 - k) * 255);
			cmyk[3].at<uchar>(i, j) = int(k * 255);
		}
	}
}


void rgb2cmyk2(Mat &src, Mat &cmyk)
{
	cmyk = Mat(src.rows, src.cols, CV_8UC4);
	Mat rgb[3];
	split(src, rgb);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			float r = float(rgb[2].at<uchar>(i, j) / 255);
			float g = float(rgb[1].at<uchar>(i, j) / 255);
			float b = float(rgb[0].at<uchar>(i, j) / 255);
			float k = min(min(r, g), b);
			cmyk.at<Vec4b>(i, j)[0] = int((1 - r - k) / (1 - k) * 255);
			cmyk.at<Vec4b>(i, j)[1] = int((1 - g - k) / (1 - k) * 255);
			cmyk.at<Vec4b>(i, j)[2] = int((1 - b - k) / (1 - k) * 255);
			cmyk.at<Vec4b>(i, j)[3] = int(k * 255);

			//cmyk[0].at<uchar>(i, j) = (1 - r - k) / (1 - k) * 255;
			//cmyk[1].at<uchar>(i, j) = (1 - g - k) / (1 - k) * 255;
			//cmyk[2].at<uchar>(i, j) = (1 - b - k) / (1 - k) * 255;
			//cmyk[3].at<uchar>(i, j) = k * 255;
		}
	}
}

void drawtheblock(Mat &src, Mat &dst, Point anchor, String str)
{
	string text = str;
	int fontFace = CV_FONT_HERSHEY_PLAIN;
	double fontScale = 1;
	int thickness = 1;
	int baseline = 0;
	Size textSize = getTextSize(text, fontFace,
		fontScale, thickness, &baseline);
	baseline += thickness;

	Mat imgroi;
	imgroi = dst(Rect(anchor.x,anchor.y, w_pic, h_pic));
	Point textOrg = anchor;
	diffcopy(src, imgroi);
	textOrg = Point(anchor.x + 10, anchor.y + 10);
	putText(dst, text, textOrg, fontFace, fontScale,
		Scalar::all(255), thickness, 8);
}

Mat histgram(Mat &src)
{
	//http://docs.opencv.org/3.0-beta/modules/imgproc/doc/histograms.html
	const int channels[1] = { 0 };
	int histSize[1] = { 255 };
	float range[] = { 0, 255 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;
	//最后创建储存直方图的矩阵:
	//下面使用OpenCV函数 calcHist 计算直方图 :
	Mat hist;
	Mat dst(src.rows, src.cols, CV_8UC3, Scalar(0, 0, 0));

	/// 计算直方图:
	calcHist(&src, 1, channels, Mat(),hist,1, histSize, &histRange,uniform,accumulate);
	normalize(hist, hist, 0, src.rows, NORM_MINMAX, -1, Mat());
	int bin_w = cvRound((double)src.cols / histSize[0]);
	for (int i = 0; i < histSize[0]; i++)
	{
		line(dst, Point(i*bin_w, src.rows),
			Point(i*bin_w, cvRound(src.rows - cvRound(hist.at<float>(i)))), Scalar(rand()%255, rand()%255, rand()%255));
	}
	//calcHist(&src)
	//calcHist(&src, 1, channels, Mat(), hist, 1, histSize, &histRange, uniform, accumulate);
	return dst;
}