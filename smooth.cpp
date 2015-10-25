#include "stdafx.h"
#include "sztx.h"

using namespace std;
using namespace cv;

Mat peper_salt(Mat&src, double persentage,float seasonor);
Mat peper(Mat&src, double persentage);
Mat salt(Mat&src, double persentage);
Mat arith_filter(Mat src,int kernal_size);
Mat geometry_filter(Mat src, int kernal_size);
Mat harmonic_filter(Mat src, int kernal_size);

static void on_geometryBlur(int, void*);
static void on_harmonicBlur(int, void*);
Mat src;
Mat final_res;
Mat polute_PaS,polute_p,polute_s;
int g_geometryBlurValue = 3, g_harmonicBlurValue = 3;

int _smooth(string img, string type)
{
	int w_pic, h_pic;
	src = imread(img);
	w_pic = src.cols;
	h_pic = src.rows;
	while (src.rows>1080 / 6 || src.cols>1920 / 6)
	{
		w_pic = w_pic * 2 / 3;
		h_pic = h_pic * 2 / 3;
		resize(src, src, Size(w_pic, h_pic));
	}
	src.convertTo(src, CV_32FC3);
	src = src / 255;
	//imshow("src", src);
	polute_PaS = peper(src, 0.05);
	polute_PaS = salt(polute_PaS, 0.05);
	polute_p = peper(src, 0.1);
	polute_s = salt(src, 0.1);

	int tmp = 3;
	string str0 = "kernal_size = ",str;
	char intstr[32];
	char c=0;

	if (type == "-example" || type == "-e")
		final_res = Mat(src.rows * 3, src.cols * 4, CV_32FC3);
	else
		final_res = Mat(src.rows * 3, src.cols * 2, CV_32FC3);
	drawtheblock(polute_PaS, final_res, Point(0, 0), "perper & salt");
	drawtheblock(polute_s, final_res, Point(0, src.rows), "only salt");
	drawtheblock(polute_p, final_res, Point(0, src.rows*2), "only peper");

	//Mat media_filter;
	//medianBlur(polute_PaS, media_filter, 5);
	//imshow("media", media_filter);
	if (type=="-arithmetic"||type=="-a")
	{
		while (c!=27)
		{
			_itoa_s(tmp, intstr,10);
			c = waitKey(1000);
			str = str0 + string(intstr);
			drawtheblock(arith_filter(polute_PaS, tmp), final_res, 
				Point(src.cols, 0), str);
			drawtheblock(arith_filter(polute_s, tmp), final_res, 
				Point(src.cols, src.rows), " ");
			drawtheblock(arith_filter(polute_p, tmp), final_res, 
				Point(src.cols, src.rows*2), " ");

			imshow("arithmetric", final_res);
			
			if (tmp == 3)
				waitKey(0);
			if (c == 32)
				c = waitKey(0);

	
			tmp += 2;
			if (tmp>src.rows/4||tmp>src.cols/4)
				break;
		}
		return 0;
	}
	else if (type=="-geometry"||type=="-g")
	{
		namedWindow("geometry");
		createTrackbar("_size", "geometry", &g_geometryBlurValue, 11, on_geometryBlur);
		on_geometryBlur(g_geometryBlurValue,0);
	}
	else if (type == "-harmonic" || type == "-h"){
		namedWindow("harmonic");
		createTrackbar("size ", "harmonic", &g_harmonicBlurValue, 11, on_harmonicBlur);
		on_harmonicBlur(g_harmonicBlurValue, 0);//Mat img = *(Mat*)usrdata;
	}
	else if (type == "-example" ||type=="-e")
	{
			drawtheblock(arith_filter(polute_PaS, tmp), final_res, 
				Point(src.cols, 0), "arithmetic");
			drawtheblock(arith_filter(polute_s, tmp), final_res, 
				Point(src.cols, src.rows), " ");
			drawtheblock(arith_filter(polute_p, tmp), final_res, 
				Point(src.cols, src.rows*2), " ");

			drawtheblock(geometry_filter(polute_PaS, tmp), final_res,
				Point(src.cols*2, 0), "geometry");
			drawtheblock(geometry_filter(polute_s, tmp), final_res,
				Point(src.cols*2, src.rows), " ");
			drawtheblock(geometry_filter(polute_p, tmp), final_res,
				Point(src.cols*2, src.rows * 2), " ");

			drawtheblock(harmonic_filter(polute_PaS, tmp), final_res,
				Point(src.cols*3, 0), "harmonic");
			drawtheblock(harmonic_filter(polute_s, tmp), final_res,
				Point(src.cols*3, src.rows), " ");
			drawtheblock(harmonic_filter(polute_p, tmp), final_res,
				Point(src.cols*3, src.rows * 2), " ");
		imshow("example", final_res);
	}
	waitKey(0);
	return 0;
}

static void on_geometryBlur(int, void*)
{	
	char intstr[30];
	string str, str0;
	str0 = "kernal_size = ";
	if (g_geometryBlurValue%2==0)
	{
		g_geometryBlurValue += 1;
	}
	_itoa_s(g_geometryBlurValue, intstr, 10);
	str = str0 + string(intstr);
	drawtheblock(geometry_filter(polute_PaS, g_geometryBlurValue), final_res,
		Point(src.cols, 0), str);
	drawtheblock(geometry_filter(polute_s, g_geometryBlurValue), final_res,
		Point(src.cols, src.rows), " ");
	drawtheblock(geometry_filter(polute_p, g_geometryBlurValue), final_res,
		Point(src.cols, src.rows * 2), " ");
	imshow("geometry", final_res);
}

static void on_harmonicBlur(int, void*){
	char intstr[30];
	string str, str0;
	str0 = "kernal_size = ";
	if (g_harmonicBlurValue%2==0)
	{
		g_harmonicBlurValue += 1;
	}
	_itoa_s(g_harmonicBlurValue, intstr, 10);
	str = str0 + string(intstr);
	drawtheblock(harmonic_filter(polute_PaS, g_harmonicBlurValue), final_res,
		Point(src.cols, 0), str);
	drawtheblock(harmonic_filter(polute_s, g_harmonicBlurValue), final_res,
		Point(src.cols, src.rows), " ");
	drawtheblock(harmonic_filter(polute_p, g_harmonicBlurValue), final_res,
		Point(src.cols, src.rows * 2), " ");

	imshow("harmonic", final_res);
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
			double sum = 1;
			for (int k = -tmp; k <= tmp; k++)
			{
				for (int l = -tmp; l <= tmp; l++)
				{
					sum *= mid_mat.at<float>(k+i, l+j);
				}
			}
			sum = pow(sum, 1.0 / (kernal_size*kernal_size));
			dst.at<float>(i-tmp, j-tmp) = (float)sum;
		}
	}
	return dst;
}

Mat harmoni_1channal(Mat src, int kernal_size){
	Mat mid_mat,dst;
	dst = Mat(src.size(), src.type());
	int tmp = (kernal_size - 1) / 2;
	copyMakeBorder(src, mid_mat, tmp, tmp, tmp, tmp, BORDER_REFLECT_101);
	for (int i = tmp; i < src.rows+tmp; i++)
	{
		for (int j = tmp; j < src.cols+tmp; j++)
		{
			double sum = 0;
			for (int k = -tmp; k <= tmp; k++)
			{
				for (int l = -tmp; l <= tmp; l++)
				{
					if (mid_mat.at<float>(k + i, l + j) == 0){
						sum = 0;
						break;
					}
					else
					{
						sum += 1.0/mid_mat.at<float>(k+i, l+j);
					}
				}
			}
			if (sum!=0)
				sum = 1.0*kernal_size*kernal_size / sum;

			dst.at<float>(i-tmp, j-tmp) = (float)sum;
		}
	}
	return dst;
}

Mat harmonic_filter(Mat src, int kernal_size){
	Mat mid_mat,dst;
	Mat bgr[3];
	if (src.channels()==1)
	{
		return harmoni_1channal(src, kernal_size);
	}
	else if (src.channels()==3)
	{
		split(src, bgr);
		bgr[0] = harmoni_1channal(bgr[0], kernal_size);
		bgr[1] = harmoni_1channal(bgr[1], kernal_size);
		bgr[2] = harmoni_1channal(bgr[2], kernal_size);
		merge(bgr, 3, dst);
		return dst;
	}
	else
	{
		cout << "the channals is invalid for this program" << endl;
	}
	return src;
}

Mat geometry_filter(Mat src, int kernal_size)
{
	Mat mid_mat,dst;
	Mat bgr[3];
	if (src.channels()==1)
	{
		return geometry_1channal(src, kernal_size);
	}
	else if (src.channels()==3)
	{
		split(src, bgr);
		bgr[0]=geometry_1channal(bgr[0], kernal_size);
		bgr[1]=geometry_1channal(bgr[1], kernal_size);
		bgr[2]=geometry_1channal(bgr[2], kernal_size);
		merge(bgr,3, dst);
		return dst;
	}
	else
	{
		cout << "the channals is invalid for this program" << endl;
	}
	return src;
}


Mat peper(Mat&src, double persentage)
{
	return peper_salt(src, persentage, 0);
}
Mat salt(Mat&src, double persentage)
{
	return peper_salt(src, persentage, 1);
}

Mat peper_salt(Mat&src, double persentage,float seasonor)
{
	int n = cvRound(persentage*src.rows*src.cols);
	int x, y,chns;
	chns = src.channels();
	Mat dst = src.clone();
	for (int k = 0; k < n; k++)
	{
		x = rand() % src.rows;
		y = rand() % src.cols;
		if (chns==1)
		{
			dst.at<float>(x, y) = seasonor;
		}
		else
		{
			for (int i = 0; i < chns; i++)
			{
				dst.at<Vec3f>(x, y)[0] = seasonor;
				dst.at<Vec3f>(x, y)[2] = seasonor;
				dst.at<Vec3f>(x, y)[1] = seasonor;
			}

		} 
	}
	return dst;

}