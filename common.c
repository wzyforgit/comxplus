#include"Kuroko.h"
#include"App.h"
#include<stdio.h>

void DealStdStr(const char *string, SerialPort *SP)
{
	if (sscanf(string, "%ld-%c-%c-%c", &SP->BaudRate, &SP->Parity, &SP->ByteSize, &SP->StopBits) != 4)
	{
		AErr = InputErr;
		return;
	}

	SP->ByteSize -= 48;
	switch (SP->Parity)
	{
	default:
		printf("未知校验类型\n");
		return;
		CASE 'N':SP->Parity = NOPARITY;
		CASE 'O':SP->Parity = ODDPARITY;
		CASE 'E':SP->Parity = EVENPARITY;
		CASE 'M':SP->Parity = MARKPARITY;
	}
	switch (SP->StopBits)
	{
	default:
		printf("停止位格式错误\n");
		return;
		CASE '1':SP->StopBits = ONESTOPBIT;
		CASE '2':SP->StopBits = TWOSTOPBITS;
		CASE '3':SP->StopBits = ONE5STOPBITS;
	}
}