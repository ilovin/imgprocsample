
#include "stdafx.h"
#include "sztx.h"

class Image
{
public:
	Image(Mat a);
	~Image();
	Mat original;
	Mat dst;

private:

};

Image::Image(Mat a)
{
	original = a;
	dst = Mat(original.size(), original.type());
}

Image::~Image()
{
}

Mat apply(Image& img,int type){
	int dim(256);
	Mat lut(1, &dim, CV_8UC(img.dst.channels()));
	switch (type)
	{
	case 0:
	if (img.dst.channels()==1)
	{
		for (int i = 0; i < 256; i++)
			lut.at<uchar>(i) = uchar(255*log2(i+1)/8);
	}
	else
	{
		for (int i = 0; i < 256; i++)
		{
			lut.at<Vec3b>(i)[0] = uchar(255 * log2(i + 1) / 8);
			lut.at<Vec3b>(i)[1] = uchar(255 * log2(i + 1) / 8);
			lut.at<Vec3b>(i)[2] = uchar(255 * log2(i + 1) / 8);
		}
	}
		break;
	case 1:
	if (img.dst.channels()==1)
	{
		for (int i = 0; i < 256; i++)
			lut.at<uchar>(i) = uchar(16 * sqrt(i + 1));
	}
	else
	{
		for (int i = 0; i < 256; i++)
		{
			lut.at<Vec3b>(i)[0] = uchar(16*sqrt(i+1));
			lut.at<Vec3b>(i)[1] = uchar(16*sqrt(i+1));
			lut.at<Vec3b>(i)[2] = uchar(16*sqrt(i+1));
		}
	}
		break;
	case 2:
	if (img.dst.channels()==1)
	{
		for (int i = 0; i < 256; i++)
			lut.at<uchar>(i) = uchar(84*pow(i,0.2));
	}
	else
	{
		for (int i = 0; i < 256; i++)
		{
			lut.at<Vec3b>(i)[0] = uchar(48*pow(i,0.3));
			lut.at<Vec3b>(i)[1] = uchar(48*pow(i,0.3));
			lut.at<Vec3b>(i)[2] = uchar(48*pow(i,0.3));
		}
	}
		break;
	default:
		break;
	}
	LUT(img.original, lut, img.dst);
	return img.dst;
}

Mat gray2Color(Mat& gray,bool method=true,int seg = 16){
	if (gray.channels()!=1)
	{
		cout << "gray2Color error,this is not a gray pic" << endl;
		return gray;
	}
	if (gray.type()==CV_32FC1)
		gray.convertTo(gray, CV_8UC1, 255);
	Mat dst(gray.size(), CV_8UC3);
	if (method)
	{
		for (int i = 0; i < gray.rows; i++)
		{
			for (int j = 0; j < gray.cols; j++)
			{
				int tmp = gray.at<uchar>(i, j);
				if (tmp<64)
				{
					dst.at<Vec3b>(i, j)[0] = 255;
					dst.at<Vec3b>(i, j)[1] = 4*tmp;
					dst.at<Vec3b>(i, j)[2] = 0;

				}
				else if (tmp<128)
				{
					dst.at<Vec3b>(i, j)[0] = 511-4*tmp;
					dst.at<Vec3b>(i, j)[1] = 255;
					dst.at<Vec3b>(i, j)[2] = 0;
				}
				else if (tmp<192)
				{
					dst.at<Vec3b>(i, j)[0] = 0;
					dst.at<Vec3b>(i, j)[1] = 255;
					dst.at<Vec3b>(i, j)[2] = 4*tmp-511;
				}
				else
				{
					dst.at<Vec3b>(i, j)[0] = 0;
					dst.at<Vec3b>(i, j)[1] = 1023-4*tmp;
					dst.at<Vec3b>(i, j)[2] = 255;
				}
			}
		}
	}
	else
	{
		int ser = 256 / seg;
		for (int i = 0; i < gray.rows; i++)
		{
			for (int j = 0; j < gray.cols; j++)
			{
				int tmp = gray.at<uchar>(i, j) / ser;
				dst.at<Vec3b>(i, j)[0] = tmp*ser;
				dst.at<Vec3b>(i, j)[1] = (seg-1-tmp)*ser;
				dst.at<Vec3b>(i, j)[2] = 0;
			}
		}

	}

	return dst;
}

static void help(){
	cout << "this is an example of lookuptable" << endl
		<< "input argument" << endl
		<< "[option][img --default template.jpg" << endl
		<< "option:" << endl
		<< "\"-log\",\"-sqrt\",\"-pow\",\"-example or -e\"" << endl;
}

int lut(string option, string img){
	Mat src = imread(img);
	if (src.empty())
	{
		cout << "cannot open the pic" << endl;
		return -1;
	}
	resizeToscreen(src, Size(1920 / 4, 1080 / 4));
	int w_pic, h_pic;
	w_pic = src.cols;
	h_pic = src.rows;

	double t = (double)getTickCount();
	Mat fin_res;
	Image imgi(src);
	if (option == "-e")
	{
		fin_res = Mat(src.rows * 2, src.cols * 2, src.type());
		drawtheblock(apply(imgi, 0), fin_res, Point(w_pic, 0), "log");
		drawtheblock(apply(imgi, 1), fin_res, Point(0, h_pic), "sqrt");
		drawtheblock(apply(imgi, 2), fin_res, Point(w_pic, h_pic), "pow_0.3");
	}
	else
		fin_res = Mat(src.rows, src.cols * 2, src.type());

	drawtheblock(src, fin_res, Point(0, 0), "original");
	if (option == "-e")
		NULL;
	else if (option=="-log")
		drawtheblock(apply(imgi, 0), fin_res, Point(w_pic, 0), "log");
	else if (option == "-sqrt")
		drawtheblock(apply(imgi, 1), fin_res, Point(w_pic, 0), "sqrt");
	else if (option == "-pow")
		drawtheblock(apply(imgi, 2), fin_res, Point(w_pic, 0), "pow_0.3");
	else if (option == "-h")
		help();
	else{
		cout << "invalid input , try again" << endl;
		help();
	}
	

	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "total time:" << t * 1000 << "ms" << endl;
	imshow("dst", fin_res);
	Mat gray;
	cvtColor(apply(imgi, 1), gray, CV_RGB2GRAY);
	Mat g2c = gray2Color(gray,true,8);
	imshow("graytoColor", g2c);
	waitKey(0);
	return 0;
}
