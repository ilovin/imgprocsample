
#include "stdafx.h"
//
using namespace std;

int sztx(string str);
int lpf(string option, string img);

int _tmain(int argc, _TCHAR* argv[])
{
	string img, option;
	option = argv[1];
	if (argc > 2)
	{
		img = argv[2];
		//src = imread(argv[1]);
	}
	else
	{
		img = "template.jpg";
		//src = imread("template.jpg");
	}
	int tmp = lpf(option,img) ;
	//int tmp = sztx(str);
	if (tmp!=0)
	{
		cout << tmp << endl;
	}
	return tmp;
}