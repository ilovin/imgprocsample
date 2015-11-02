
#include "stdafx.h"
#include "sztx.h"

class Image
{
public:
	Image(Mat a, Mat b);
	~Image();
	Mat original;
	Mat dst;

private:

};

Image::Image(Mat a,Mat b)
{
	original = a;
	dst = b;
}

Image::~Image()
{
}

int apply(Image& img){
	int dim(256);
	Mat lut(1, &dim, CV_8UC(img.dst.channels()));
	if (img.dst.channels()==1)
	{
		for (int i = 0; i < 256; i++)
			lut.at<uchar>(i) = 255 - i;
	}
	else
	{
		for (int i = 0; i < 256; i++)
		{
			lut.at<Vec3b>(i)[0] = 255 - i;
			lut.at<Vec3b>(i)[1] = 255 - i;
			lut.at<Vec3b>(i)[2] = 255 - i;
		}
	}
	LUT(img.original, lut, img.dst);
	return 0;
}

int lut(string option, string img){
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
	Image imgi(src, final_res);
	cout << apply(imgi) << endl;
	imshow("dst", imgi.dst);
	waitKey(0);
	return 0;
}
