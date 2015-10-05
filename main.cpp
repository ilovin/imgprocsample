
#include "stdafx.h"
//
using namespace std;

int sztx(string str);
int lpf(string option, string img);

int _tmain(int argc, _TCHAR* argv[])
{
	string img, option;
	if (argc<2)
		option = "--h";
	else
		option = argv[1];

	if (argc > 2)
		img = argv[2];
	else
		img = "template.jpg";

	int tmp = lpf(option,img) ;

	//for sztx.cpp
	//int tmp = sztx(option);
	if (tmp!=0)
	{
		cout << tmp << endl;
	}
	return tmp;
}