// sztx.cpp : 定义控制台应用程序的入口点。
// 入口：输入一个图片的str
// 出口：在本目录下生成一个bmp图片，将图片split成rgb，并且分别均衡化后在合成一幅图片
// 二值化，以及绘制出灰度图，还有均衡化过的直方图
//

#include "stdafx.h"
#include "sztx.h"

//using namespace std;
//using namespace cv;
//
//void diffcopy(Mat &src, Mat &dst);
//void rgb2cmyk(Mat &src, vector<Mat> &cmyk);
//void rgb2cmyk2(Mat &src, Mat &cmyk);
//void drawtheblock(Mat &src,Mat &dst, Point anchor, String str);
//Mat histgram(Mat &src);
//Mat histgram3c(Mat &src);
//Mat equalize3c(Mat &src);

static int w_pic = int(1920 / 6), h_pic = int(1080 / 6);
int sztx(string str)
{
	Mat src;
	src = imread(str);
	if (src.empty())
	{
		return -1;
	}
	imwrite("dst.bmp", src);
	resize(src, src, Size(w_pic, h_pic));
	//cout << src.size() << endl;
	//imshow("src", src);
	Mat gray,binary,rgb[3],hsv;
	cvtColor(src, gray, CV_BGR2GRAY);
	cvtColor(src, hsv, CV_BGR2HSV);
	threshold(gray, binary,0, 255,THRESH_BINARY|THRESH_OTSU);
	split(src, rgb);
	Mat dst(h_pic * 3, w_pic * 4,CV_8UC3);
	Mat cmyk;
	Mat eqh;
	Mat equa3c;
	equa3c = equalize3c(src);
	equalizeHist(gray, eqh);

	//histgram3c(src);
	waitKey(0);
	//paint the pic
	drawtheblock(src, dst, Point(w_pic, 0), "org");
	drawtheblock(gray, dst, Point(w_pic, h_pic), "gray");
	drawtheblock(binary, dst, Point(w_pic * 2, 0), "binary");
	drawtheblock(rgb[2], dst, Point(0, 0), "r");
	drawtheblock(rgb[1], dst, Point(0, h_pic), "g");
	drawtheblock(rgb[0], dst, Point(0, h_pic*2), "b");
	drawtheblock(eqh, dst, Point(w_pic*2, h_pic), "equalizehist");
	drawtheblock(histgram(gray), dst, Point(w_pic, h_pic*2), "before");
	drawtheblock(histgram(eqh), dst, Point(w_pic * 2, h_pic * 2), "after");
	drawtheblock(equa3c, dst, Point(w_pic * 3, 0), "equalize3c");

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
	if (src.type ()!=dst.type())
	{
		if (dst.type()==CV_32FC3)
		{
			if (src.type() == CV_8UC3)
			{
				src.convertTo(tmp, CV_32FC3);
				normalize(tmp, tmp, 0, 1, NORM_MINMAX);
				tmp.copyTo(dst);
			}
			else if (src.type() == CV_8UC1)
			{
				src.convertTo(tmp, dst.type());
				normalize(tmp, tmp, 0, 1, NORM_MINMAX);
				cvtColor(tmp, tmp, CV_GRAY2BGR);
				tmp.copyTo(dst);
			}
			else
			{
				cvtColor(src, tmp, CV_GRAY2BGR);
				tmp.copyTo(dst);
			}
		}
		else if (dst.type() == CV_8UC3)
		{
			cvtColor(src, tmp, CV_GRAY2BGR);
			tmp.copyTo(dst);
		}
		else
		{
			cout << "invalid type for copy picture" << endl;
		}
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

Mat histgram3c(Mat &src,Size output_size)
{
	//Mat dst(src.rows, src.cols, CV_8UC3);
	vector<Mat>bgr_planes;
	split(src, bgr_planes);
	int histSize = 256;
	//int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;

	int channels[] = { 0, 1, 2 };

	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	// http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
	//  &bgr_planes[0]: The source array(s)
	//	1 : The number of source arrays(in this case we are using 1. We can enter here also a list of arrays)
	//	0 : The channel(dim) to be measured.In this case it is just the intensity(each array is single - channel) so we just write 0.
	//	Mat(): A mask to be used on the source array(zeros indicating pixels to be ignored).If not defined it is not used
	//b_hist : The Mat object where the histogram will be stored
	//		 1 : The histogram dimensionality.
	//	 histSize : The number of bins per each used dimension
	//			histRange : The range of values to be measured per each dimension
	//						uniform and accumulate : The bin sizes are the same and the histogram is cleared at the beginning.

	//calcHist(&src, 1,channels, Mat(), thist, 3, histSize, histRange, uniform, accumulate);
	//int hist_w = 512; int hist_h = 400;
	int hist_w = output_size.width;
	int hist_h = output_size.height;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	//for (int i = 1; i < histSize[0]-1; i++)
	//{
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
	//		Scalar(255, 0, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
	//		Scalar(0, 255, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
	//		Scalar(0, 0, 255), 2, 8, 0);
	//}

	/// Display

	//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	//imshow("calcHist Demo", histImage);

	//calcHist(&src)
	//calcHist(&src, 1, channels, Mat(), hist, 1, histSize, &histRange, uniform, accumulate);
	return histImage;
}

Mat equalize3c(Mat &src)
{
	vector<Mat>bgr_planes;
	split(src, bgr_planes);
	Mat b, g, r;
	vector<Mat>bgr_equa;

	equalizeHist(bgr_planes[0], b);
	equalizeHist(bgr_planes[1], g);
	equalizeHist(bgr_planes[2], r);
	bgr_equa = { b, g, r };
	//bgr_equa.push_back(b);
	//bgr_equa.push_back(g);
	//bgr_equa.push_back(r);
	Mat dst;
	merge(bgr_equa, dst);
	//imshow("equalized", dst);
	return dst;
}