#ifndef _EKU_CONCOL
#define _EKU_CONCOL

#include <Windows.h>
#include <iostream>
bool textcolorprotect=true;
/*doesn't let textcolor be the same as backgroung color if true*/

enum concol
{
	black=0,
	dark_blue=1,
	dark_green=2,
	dark_aqua,dark_cyan=3,
	dark_red=4,
	dark_purple=5,dark_pink=5,dark_magenta=5,
	dark_yellow=6,
	dark_white=7,
	gray=8,
	blue=9,
	green=10,
	aqua=11,cyan=11,
	red=12,
	purple=13,pink=13,magenta=13,
	yellow=14,
	white=15
};

inline void setcolor(concol textcolor,concol backcolor);
inline void setcolor(int textcolor,int backcolor);
int textcolor();/*returns current text color*/
int backcolor();/*returns current background color*/

#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)

//-----------------------------------------------------------------------------

int textcolor()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(std_con_out,&csbi);
	int a=csbi.wAttributes;
	return a%16;
}

int backcolor()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(std_con_out,&csbi);
	int a=csbi.wAttributes;
	return (a/16)%16;
}

inline void setcolor(concol textcol,concol backcol)
{setcolor(int(textcol),int(backcol));}

inline void setcolor(int textcol,int backcol)
{
	if(textcolorprotect)
	{if((textcol%16)==(backcol%16))textcol++;}
	textcol%=16;backcol%=16;
	unsigned short wAttributes= ((unsigned)backcol<<4)|(unsigned)textcol;
	SetConsoleTextAttribute(std_con_out, wAttributes);
}

#endif /*_EKU_CONCOL*/ 