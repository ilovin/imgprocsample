
#include "stdafx.h"
//
using namespace std;

int sztx(string str);

int _tmain(int argc, _TCHAR* argv[])
{
	string str;
	if (argc > 1)
	{
		str = argv[1];
		//src = imread(argv[1]);
	}
	else
	{
		str = "template.jpg";
		//src = imread("template.jpg");
	}
	int tmp = sztx(str);
	if (tmp!=0)
	{
		cout << tmp << endl;
	}
	return tmp;

}