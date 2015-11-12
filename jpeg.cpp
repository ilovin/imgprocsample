
#include "stdafx.h"
#include <windows.h>  
#include <stdlib.h>
#include "sztx.h"
extern "C"{
	#include "jpeglib.h"
};
#include <setjmp.h>
#include <iostream>
#include <io.h>

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
JSAMPLE * image_buffer;
int width = 0;
int height = 0;

void write_JPEG_file(char * filename, int quality);

int jpeg(string option, string img){
	BITMAPFILEHEADER bfh;
	BITMAPINFO bi;
	BITMAPINFOHEADER *bih;

	
	FILE *infile;
	//infile = fopen(img.c_str(), "rb");
	fopen_s(&infile, img.c_str(), "rb");
	if (!infile){
		cout << "cannot open the file" << endl;
		cin.get();
		return -1; 
	}
	//read bmp file header
	fread_s(&bfh, sizeof(BITMAPFILEHEADER), 1,
		sizeof(BITMAPFILEHEADER), infile);
	//read info header
	fread_s(&bi, sizeof(BITMAPINFO), 1, sizeof(BITMAPINFO), infile);

	bih = &bi.bmiHeader;
	width = bih->biWidth;
	height = bih->biHeight;
	int data_size = width*height * 3;

	JSAMPLE *srcbuf = (JSAMPLE*)malloc(data_size);
	fread_s(srcbuf, data_size, 1, data_size, infile);
	fclose(infile);

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)
		padding++;
	int psw = scanlinebytes + padding;

	image_buffer = (JSAMPLE*)malloc(height*psw);

	int quality = 60;
	if (option=="-opencv"|option=="-o"|option=="-e")
	{
		Mat src = imread(img);
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				image_buffer[i*src.cols * 3 + j * 3] = src.at<Vec3b>(i, j)[2];
				image_buffer[i*src.cols * 3 + j * 3 + 1] = src.at<Vec3b>(i, j)[1];
				image_buffer[i*src.cols * 3 + j * 3 + 2] = src.at<Vec3b>(i, j)[0];
			}
		}
	}
	else
	{
		long bufpos = 0;
		long newpos = 0;
		for (int y = 0; y < height; y++)
			for (int x = 1; x < 3 * width; x += 3)
			{
				bufpos = y * 3 * width + x;     // position in original buffer
				newpos = (height - y - 1) * psw + x;// position in padded buffer
				image_buffer[newpos] = srcbuf[bufpos + 2];       // swap r and b
				image_buffer[newpos + 1] = srcbuf[bufpos + 1]; // g stays
				image_buffer[newpos + 2] = srcbuf[bufpos];     // swap b and r
			}
	}

	cout << "please enter a number of the quality [0-100]" <<
		"--default:<60>" << endl;
	cin >> quality;
	write_JPEG_file("dst.jpg", quality);
	fopen_s(&infile, img.c_str(), "rb");
	cout << "the original size is:" << _filelength(_fileno(infile)) << endl;
	fopen_s(&infile,"dst.jpg" , "rb");
	cout << "the dst size is:" << _filelength(_fileno(infile)) << endl;

	cin.get();
	cin.get();
	//upside down
	//for (int i = 0; i < height/2; i++)
	//{
	//	for (int j = 0; j < width*3; j++)
	//	{
	//		tmp = image_buffer[width*i*3 + j];
	//		image_buffer[width*i*3 + j] = image_buffer[width*3*(height - 1 - i) + j];
	//		image_buffer[width*3*(height - 1 - i) + j] = tmp;
	//	}
	//}

	//for (int i = 0; i < data_size/2; i++)
	//{
	//	tmp = image_buffer[data_size - 1 - i];
	//	image_buffer[data_size - 1 - i] = image_buffer[i];
	//	image_buffer[i] = tmp;
	//}
		//for (int i = 2; i < data_size; i += 3)
		//{
		//	tmp = image_buffer[i - 2];
		//	image_buffer[i - 2] = image_buffer[i];
		//	image_buffer[i] = tmp;
		//}

	//for (int i = 0; i < src.rows; i++)
	//{
	//	for (int j = 0; j < src.cols; j++)
	//	{

	//	}
	//}


	//Mat src = imread(img);
	//if (src.empty()) return -1;
	////resizeToscreen(src, Size(1920 / 3, 1080 / 3));
	//Mat deal = src.clone();
	//copyMakeBorder(deal, deal, 0, deal.rows % 8, 0, deal.cols % 8, BORDER_REFLECT101);
	//imshow("src", deal);
	//deal.convertTo(deal, CV_BGR2YUV);
	//deal.convertTo(deal, CV_32FC3);
	////deal = (Mat_<float>&)deal;
	//cout << deal.at<Vec3f>(0, 0);
	//deal -= Scalar::all(128);
	//vector<Mat>planes;
	//split(deal, planes);
	////comprass
	//for (int i = 1; i < (int)planes.size(); i++)
	//{
	//	resize(planes[i], planes[i], Size(deal.cols / 2, deal.rows / 2));
	//	resize(planes[i], planes[i], Size(deal.cols, deal.rows));
	//}
	//Mat quant = Mat(Size(8, 8), CV_32F, &data);
	//for (int i = 0; i < deal.rows; i+=8)
	//{
	//	for (int  j = 0; j < deal.cols; j+=8)
	//	{
	//		for (int k = 1; k < (int)planes.size(); k++)
	//		{
	//			Mat imgroi = planes[k](Rect(j,i,8,8));
	//			dct(imgroi, imgroi);
	//			divide(imgroi, quant, imgroi);
	//		}
	//	}
	//}
	//for (int i = 0; i < deal.rows; i+=8)
	//{
	//	for (int  j = 0; j < deal.cols; j+=8)
	//	{
	//		for (int k = 1; k < (int)planes.size(); k++)
	//		{
	//			Mat imgroi = planes[k](Rect(j, i, 8, 8));
	//			multiply(imgroi, quant, imgroi);
	//			idct(imgroi, imgroi);
	//		}
	//	}
	//}
	//merge(planes, deal);
	//deal += Scalar::all(128);
	//deal.convertTo(deal, CV_YUV2RGB);
	////deal /= 255;
	//deal.convertTo(deal, CV_8UC3);
	////deal = (Mat_<uchar>&)deal;
	//Mat dst = deal(Rect(0, 0, src.cols, src.rows));
	//Mat diff = src - dst;
	//diff *= 16;
	//imshow("diff", diff);
	////imshow("dst", dst);
	//vector<int>params;
	//params.push_back(IMWRITE_JPEG_QUALITY);
	//params.push_back(60);
	//imwrite("dst.jpg", dst,params);

	//waitKey(0);

	return 0;
}



void write_JPEG_file(char * filename, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;
	JSAMPROW row_pointer[1];
	int row_stride;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
   
	//fopen_s(&outfile, filename, "wb");
	if ((outfile = fopen(filename, "wb+")) == NULL)
	{
		cout << stderr<<" cannot open the target file : " <<filename<< endl;
		cin.get();
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, quality, true);
	jpeg_start_compress(&cinfo, true);

	//jpeg_stdio_src(&cinfo, infile);
	//jpeg_stdio_src(&cinfo, infile);
	row_stride = width * 3;
	while (cinfo.next_scanline<cinfo.image_height)
	{
		row_pointer[0] = &image_buffer[cinfo.next_scanline*row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
}
