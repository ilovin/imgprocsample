
#include "stdafx.h"
//
using namespace std;

int sztx(string str);
int lpf(string option, string img);
int dwt(string img, int time);
int _smooth(string img, string type);
int lut(string option, string img);
int jpeg(string option, string img);
int linedetect(string option, string img);

int _tmain(int argc, _TCHAR* argv[])
{
	string img,option;
	int tmp;
	img = argc > 2 ? argv[2] : "template.jpg";
	option = argc > 1 ? argv[1] : "-e";
	tmp = linedetect(option, img);
	if (tmp!=0)
	{
		cout << tmp << endl;
	}
	return tmp;
}

//for lut
//img = argc > 2 ? argv[2] : "black.png";
//option = argc > 1 ? argv[1] : "-e";


//for dwt
//if (argc>2)
//{
//	option = argv[2];
//	img = argv[1];
//}
//else
//{
//	option = "1";
//	if (argc < 2)
//		img = "template.jpg";
//	else
//		img = argv[1];
//}

//for low pass filter
//string option
//if (argc<2)
//	option = "--h";
//else
//	option = argv[1];

//if (argc > 2)
//	img = argv[2];
//else
//	img = "template.jpg";

//int tmp = lpf(option,img) ;

//for sztx.cpp
//int tmp = sztx(option);