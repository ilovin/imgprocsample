
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
	high_thresh = threshold(gray, binary, 0, 255, 
		THRESH_BINARY | THRESH_OTSU);
	low_thresh = high_thresh / 2;
	Canny(gray, canny, low_thresh, high_thresh);
	return canny;
}
Mat Image::getbinaryCanny()
{
	Canny(binary, canny, 64, 128);
	return canny;
}
Mat Image::getGray()
{
	return gray;
}

static void help()
{
	cout << "this program will detect the black and"
		<< " bold line of the piture" << endl
		<< "How to use:" << endl
		<< "in cmd:sztx.exe [img --default template.png]" << endl;
}

int linedetect(string img){
	Mat src = imread(img);
	if (src.empty())
	{
		help();
		cin.get();
		return -1;
	}
	resizeToscreen(src, Size(1920 / 3, 1080 / 3));
	//src = equalize3c(src);
	src = src(Rect(0, src.rows/4, src.cols, src.rows/4*3));
	Mat dst = Mat(src.rows * 2, src.cols * 2, src.type());
	drawtheblock(src, dst, Point(0, src.rows),"src");
	medianBlur(src, src, 11);
	//GaussianBlur(src, src, Size(7, 7), 3, 3);
	Mat kernal = getStructuringElement(MORPH_RECT,Size(7,7),Point(3,3));
	//morphologyEx(src, src, MORPH_OPEN, kernal);
	Image pic = Image(src);
	//imshow("hist", histgram3c(src,src.size()));
	Mat binary;
	int blocksize = 301;
	adaptiveThreshold(pic.getGray(), binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
		THRESH_BINARY_INV, blocksize, 5);
	drawtheblock(binary, dst, Point(0, 0), "adaptivethresh_301");
	//threshold(pic.getGray(), binary, 50, 255, THRESH_BINARY);
	Mat kernal2 = getStructuringElement(MORPH_RECT,Size(3,6),Point(1,3));
	morphologyEx(binary, binary, MORPH_OPEN, kernal2);
	Mat kernal1 = getStructuringElement(MORPH_RECT,Size(6,3),Point(3,1));
	morphologyEx(binary, binary, MORPH_OPEN, kernal1);
	pic.binary = binary;
	drawtheblock(binary, dst, Point(src.cols, 0), "morpholoty");
	//imshow("binary", binary);

	//imwrite("write.jpg", src);
	vector<Vec4i> lines;
	HoughLinesP(pic.getbinaryCanny(), lines, 1, CV_PI / 180, min(src.rows,src.cols)/3,
		min(src.rows, src.cols) / 3, min(src.rows, src.cols) / 8);
	//imshow("binary", binary);
	for (int i = 0; i < (int)lines.size(); i++)
	{
		Scalar color;
		color[0] = rand() % 255;
		color[1] = rand() % 255;
		color[2] = rand() % 255;
		line(src, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]),
			Scalar::all(255));
	}
	drawtheblock(src, dst, Point(src.cols, src.rows), "linedetect");
	imshow("dst", dst);
	//imshow("line detect", src);
	//imshow("binary canny", pic.getbinaryCanny());
	//imshow("canny", pic.getCanny());

	waitKey();
	return 0;
}