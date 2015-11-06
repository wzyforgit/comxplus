#include"Kuroko.h"
#include"App.h"
#include<windows.h>
#include<process.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

static unsigned int WINAPI ReadInfo(PVOID Hcom)
{
	char Buf;
	while (!fExit)
	{
		if (ReMessege(Hcom, &Buf))
		{
			printf("%c", Buf);
		}
		else
		{
			if (Err == Err_PullOut)
			{
				return EXIT_SUCCESS;
			}
		}
	}
	return EXIT_SUCCESS;
}

static unsigned int WINAPI WriteInfo(PVOID Hcom)
{
	char Buf;
	while (1)
	{
		Buf = _getch();
		if (Buf == 27)
		{
			fExit = 1;
			break;
		}
		if (!SeMessege(Hcom, &Buf))
		{
			return EXIT_SUCCESS;
		}
	}
	return EXIT_SUCCESS;
}

static void HyperTerminal(SerialPort SP)
{
	SP.Hcom = SerialPortInit(SP);
	CheckComHandle(SP.Hcom);
	HANDLE Wait[2];
	Wait[0] = (HANDLE)_beginthreadex(NULL, 0, WriteInfo, SP.Hcom, 0, NULL);
	Wait[1] = (HANDLE)_beginthreadex(NULL, 0, ReadInfo, SP.Hcom, 0, NULL);
	WaitForMultipleObjects(2, Wait, TRUE, INFINITE);
	CloseHandle(SP.Hcom);
	CloseHandle(Wait[0]);
	CloseHandle(Wait[1]);
	fExit = 0;
}

void HTforCMD(int argc, char **argv)
{
	if (argc < 4)
	{
		AErr = FewParameter;
		return;
	}

	SerialPort SP = {
		.ComName = { 'C', 'O', 'M' }
	};

	if (strlen(argv[2])>3)
	{
		AErr = SerialPortNameErr;
		return;
	}
	sprintf(&SP.ComName[3], argv[2]);
	
	DealStdStr(argv[3], &SP);
	
	HyperTerminal(SP);
}