
#include "stdafx.h"
//
using namespace std;

int sztx(string str);
int lpf(string option, string img);
int dwt(string img, int time);
int _smooth(string img, string type);
int lut(string option, string img);

int _tmain(int argc, _TCHAR* argv[])
{
	string img,option;
	int tmp;

	switch (argc)
	{
	case 1:
		img = "black.png";
		option = "-e";
		break;
	case 2:
		img = "black.png";
		option = argv[1];
		break;
	case 3:
		img = argv[2];
		option = argv[1];
		break;
	default:
		break;
	}
	tmp = lut(option,img);
	//tmp = dwt(img, atoi(option.c_str()));
	if (tmp!=0)
	{
		cout << tmp << endl;
	}
	return tmp;
}

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