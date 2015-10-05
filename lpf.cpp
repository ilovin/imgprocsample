
#include "stdafx.h"
#include "sztx.h"
//using namespace std;
//using namespace cv;

static void help(void);
void shiftDFT(Mat &src);
void DFT(Mat &src, Mat &dst);
Mat show_spectrum_magnitude(Mat &complexImg, bool shift = false);
Mat createIdealfilter(Size size_of_filter, int D);
Mat createGaussianfilter(Size size_of_filter, double sigma_h, double sigma_v);
Mat createbutterworthfilter(Size size_of_filter, int D, int order);
Mat do_filter(Mat &dft_result, Mat filter);
Mat draw_the_map(Mat r_dft_result, Mat g_dft_result, Mat b_dft_result, Mat filter, 
	int type,Point anchor=Point(0,0),string str="str");

int w_pic = int(1920 / 6), h_pic = int(1080 / 6);
Mat dst(h_pic * 3, w_pic * 3, CV_32FC3);

int lpf(string option, string img){
	Mat src = imread(img);
	Mat bgr[3];

	if (src.empty())
	{
		return -1;
	}
	if (option=="--help"||option=="--h")
	{
		help();
		cout << "press any key to continue" << endl;
		cin.get();
		return 0;
	}
	resize(src, src, Size(w_pic, h_pic));
	//imshow("src", src);
	split(src, bgr);
	drawtheblock(bgr[0], dst, Point(0, 0), "b");
	drawtheblock(bgr[1], dst, Point(0, h_pic), "g");
	drawtheblock(bgr[2], dst, Point(0, h_pic * 2), "r");
	//imshow("final", dst);

	int type = 0;
	if (strcmp(option.c_str(),"--ideal")==0||strcmp(option.c_str(),"--i")==0)
	{
		;
	}
	else if (strcmp(option.c_str(), "--gaussian")==0||strcmp(option.c_str(),"--g")==0)
	{
		type = 1;
	}
	else if (strcmp(option.c_str(), "--butterworth")==0||strcmp(option.c_str(),"--b")==0)
	{
		type = 2;
	}
	else if (option=="--example"||option=="--e")
	{
		type = 3;
	}
	else
	{
		cout << "invalid option,please check again" << endl;
		return -1;
	}

	Mat dft_result,r_dft_result,g_dft_result,b_dft_result;
	DFT(bgr[0], b_dft_result);
	shiftDFT(b_dft_result);
	DFT(bgr[1], g_dft_result);
	shiftDFT(g_dft_result);
	DFT(bgr[2], r_dft_result);
	shiftDFT(r_dft_result);
	if (type!=3)
	{
		drawtheblock(show_spectrum_magnitude(r_dft_result, false), dst, 
			Point(w_pic, 0), "r_dft");
		drawtheblock(src, dst, Point(w_pic*2, 0), "origin");
		drawtheblock(histgram3c(src,src.size()), dst, 
			Point(w_pic * 2, h_pic*2), "the histgram of src");
	}
	else
	{
		imshow("src", src);
	}

	Mat filter;
	switch (type)
	{
	case 0:
	{
		cout << "Using Ideal filter now,please enter a value <filter radius>"
		<<"that is less than min(rows,cols)/2 :" 
		<<min(src.rows,src.cols)/2<< endl
		<<"if it is non positive,radius = min(src.rows,src.cols)/4" << endl;
		int tmp;
		cin >> tmp;
		filter = createIdealfilter(r_dft_result.size(), tmp);
		draw_the_map(r_dft_result, g_dft_result, b_dft_result, filter,0);
	}
	break;

	case 1:
	{
		cout << "Using gaussian filter now,please enter the <sigma>s for"
		<<"horizontal & vertical" << endl 
		<<"if it is non positive,sigma=0.3*((ksize-1)*0.5 - 1) + 0.8" << endl
		<< "sigma should be less than min(dft_result.rows,dft_result.cols),"
		<<"which is : "<<min(src.rows,src.cols)<<endl;
		double sigma_h,sigma_v;
		cin >> sigma_h>>sigma_v;
		filter = createGaussianfilter(src.size(), sigma_h,sigma_v);
		draw_the_map(r_dft_result, g_dft_result, b_dft_result, filter,1);
		//imshow("dst", do_filter(dft_result, filter));
	}
	break;

	case 2:
	{
		cout << "Using butterworth filter now,please input the <D_0> & <order>,"
		"the less,the stronger" << endl
		<< "D_0 should less than min(src.rows,src.cols),which is :" 
		<< min(src.rows, src.cols) << endl;
		int d_0, order;
		cin >> d_0>>order;
		filter = createbutterworthfilter(src.size(), d_0, order);
		draw_the_map(r_dft_result, g_dft_result, b_dft_result, filter,2);
		//imshow("dst", do_filter(dft_result, filter));
	}
	break;

	case 3:
	{
		cout << "this is an example of the low pass filter" << endl;
		Mat r, g, b;
		r = r_dft_result.clone();
		g = g_dft_result.clone();
		b = b_dft_result.clone();
		filter = createIdealfilter(r_dft_result.size(), 50);
		draw_the_map(r, g, b, filter, 3, Point(w_pic, 0), "ideal");
		r = r_dft_result.clone();
		g = g_dft_result.clone();
		b = b_dft_result.clone();
		filter = createGaussianfilter(r_dft_result.size(), 50,50);
		draw_the_map(r, g, b, filter, 3, Point(w_pic, h_pic), "Gaussian");
		r = r_dft_result.clone();
		g = g_dft_result.clone();
		b = b_dft_result.clone();
		filter = createbutterworthfilter(src.size(), 60, 3);
		draw_the_map(r, g, b, filter, 3, Point(w_pic, h_pic * 2), "butterworth");
	}
	break;
	default:
		break;
	}
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

Mat draw_the_map(Mat r_dft_result, Mat g_dft_result, Mat b_dft_result,Mat filter,
	int type,Point anchor,string str)
{
	Mat r_result, g_result, b_result;
	Mat final_result;

	r_result = do_filter(r_dft_result, filter);
	g_result = do_filter(g_dft_result, filter);
	b_result = do_filter(b_dft_result, filter);
	//paint
	if (type!=3)
	{
		drawtheblock(show_spectrum_magnitude(r_dft_result, true), dst, Point(w_pic, h_pic), "r_filter after");
		drawtheblock(r_result, dst, Point(w_pic, h_pic * 2), "r_filter");
	}
	else
	{
		drawtheblock(show_spectrum_magnitude(r_dft_result, true), dst, anchor,str);
	}

	Mat planes[3] = { b_result, g_result, r_result };
	merge(planes, 3, final_result);

	if (type!=3)
	{
		drawtheblock(final_result, dst, Point(w_pic * 2, h_pic), "final_filter");
	}
	else
	{
		drawtheblock(final_result, dst, Point(anchor.x + w_pic, anchor.y), " ");
	}
	return final_result;
}

Mat do_filter(Mat &dft_result, Mat filter)
{
	mulSpectrums(dft_result, filter, dft_result, DFT_ROWS);

	Mat dst;
	//imshow("after filter the dft", show_spectrum_magnitude(dft_result, false));

	shiftDFT(dft_result);//之前交换过，现在交换回来
	idft(dft_result, dst);
	normalize(dst, dst, 0, 1, CV_MINMAX);
	//imshow("after filter the dft",show_spectrum_magnitude(dft_result, false));
	Mat planes[2];
	split(dst, planes);
	dst = planes[0];
	return dst;
}

//D截至频率距离原点处的距离，
Mat createbutterworthfilter(Size size_of_filter, int D, int order)
{
	Mat filter(size_of_filter,CV_32F,Scalar::all(0));
	Point center(size_of_filter.width / 2, size_of_filter.height / 2);
	double radius;
	for (int i = 0; i < size_of_filter.height; i++)
	{
		for (int j = 0; j < size_of_filter.width; j++)
		{
			radius = sqrt(pow(j - center.x, 2.0) + pow(i - center.y, 2.0));
			filter.at<float>(i, j) = (float)(1 / (1 + pow(radius/D, 2 * order)));
		}
	}
	Mat planes[2] = { filter, filter };
	merge(planes, 2, filter);
	return filter;
}

Mat createGaussianfilter(Size size_of_filter, double sigma_h,double sigma_v)
{
	double sig_h,sig_v;
	if (sigma_h<=0)
	{
		sig_h = 0.3*((size_of_filter.width - 1)*0.5 - 1) + 0.8;
	}
	else
	{
		sig_h = sigma_h;
	}
	if (sigma_h <= 0)
	{
		sig_v = 0.3*((size_of_filter.height - 1)*0.5 - 1) + 0.8;
	}
	else
	{
		sig_v = sigma_v;
	}
	Mat gaussianFilter(size_of_filter,CV_32F,Scalar::all(0));
	Mat filter_w = getGaussianKernel(size_of_filter.width, sig_h,CV_32F);
	Mat filter_h = getGaussianKernel(size_of_filter.height, sig_v,CV_32F);
	gaussianFilter = filter_h*filter_w.t();
	normalize(gaussianFilter, gaussianFilter, 0, 1, NORM_MINMAX);

	Mat planes[2] = { gaussianFilter, gaussianFilter };
	merge(planes, 2, gaussianFilter);
	return gaussianFilter;
}

Mat createIdealfilter(Size size_of_filter, int D)
{
	int radius = 0;
	if (D>0)
	{
		radius = D;
	}
	else
	{
		radius = min(size_of_filter.width, size_of_filter.height) / 4;
	}
	Mat filter(size_of_filter, CV_32F, Scalar::all(0));
	circle(filter, Point(size_of_filter.width / 2, size_of_filter.height / 2), radius, Scalar::all(1), -1);
	Mat to_merge[] = { filter, filter };
	merge(to_merge, 2, filter);
	return filter;
}

Mat show_spectrum_magnitude(Mat &complexImg,bool shift)
{
	Mat planes[2], dst;
	split(complexImg, planes);
	magnitude(planes[0], planes[1], dst);
	dst += Scalar::all(1);
	log(dst, dst);
	if (shift)
	{
		shiftDFT(dst);
	}
	normalize(dst, dst, 0, 1, CV_MINMAX);
	return dst;
}

void DFT(Mat &src, Mat &dst)
{
	copyMakeBorder(src, dst, 0, getOptimalDFTSize(src.rows) - src.rows,
		0, getOptimalDFTSize(src.cols) - src.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(dst), Mat::zeros(dst.size(), CV_32F) };
	merge(planes, 2, dst);
	dft(dst, dst);
}

static void help(void){
	cout
		<< "[option][img_path -- default template.jpg]" << endl
		<< "options : " << endl
		<< "\t--e : an example" << endl
		<<"\t--i : ideal low pass filter"<<endl
		<<"\t--g : gaussian LPF"<<endl
		<<"\t--b : butterworth LPF" << endl;
}

void shiftDFT(Mat &src)
{
	src = src(Rect(0, 0, src.cols&-2, src.rows&-2));
	int cx = src.cols / 2;
	int cy = src.rows / 2;
	Mat tmp, q0, q1, q2, q3;
	q0 = src(Rect(0, 0, cx, cy));
	q1 = src(Rect(cx, 0, cx, cy));
	q2 = src(Rect(cx, cy, cx, cy));
	q3 = src(Rect(0, cy, cx, cy));
	q0.copyTo(tmp);
	q2.copyTo(q0);
	tmp.copyTo(q2);
	q1.copyTo(tmp);
	q3.copyTo(q1);
	tmp.copyTo(q3);
}


