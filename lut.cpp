
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
	//cout << "dst.channal : " << img.dst.channels() << endl;
	//switch (type)
	//{
	//case 0:
	//	break;
	//case 1:
	//	break;
	//case 2:
	//	break;
	//default:
	//	break;
	//}
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

int lut(string option, string img){
	int w_pic, h_pic;
	Mat src = imread(img);
	w_pic = src.cols;
	h_pic = src.rows;
	while (src.rows>1080 / 3 || src.cols>1920 / 3)
	{
		w_pic = w_pic * 2 / 3;
		h_pic = h_pic * 2 / 3;
		resize(src, src, Size(w_pic, h_pic));
	}
	double t = (double)getTickCount();
	Mat fin_res(src.rows*2,src.cols*2,src.type());
	drawtheblock(src, fin_res, Point(0, 0), "original");
	Image imgi(src);
	drawtheblock(apply(imgi, 0), fin_res, Point(w_pic, 0), "log");
	drawtheblock(apply(imgi, 1), fin_res, Point(0, h_pic), "sqrt");
	drawtheblock(apply(imgi, 2), fin_res, Point(w_pic, h_pic), "pow_0.3");
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "total time:" << t << endl;
	//cout << apply(imgi,0) << endl;
	imshow("dst", fin_res);
	waitKey(0);
	return 0;
}
