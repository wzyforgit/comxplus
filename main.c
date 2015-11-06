#include"Kuroko.h"
#include"App.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum A_Error AErr = None;

void DisplayError(void)
{
	if (Err != NoErr)
	{
		switch (Err)
		{
			CASE Err_OpenCom : printf("串口打开失败\n");
			CASE Err_SetTimeOuts : printf("设置超时失败\n");
			CASE Err_SetBuffer : printf("设置缓冲区失败\n");
			CASE Err_GetDCB : printf("获取DCB失败\n");
			CASE Err_SetDCB : printf("设置DCB失败\n");
			CASE Err_ClearBuf : printf("清除缓冲区失败\n");
			CASE Err_PullOut : printf("串口被拔出\n");
		}
	}

	if (AErr != NoErr)
	{
		switch (AErr)
		{
			CASE FewParameter : printf("参数过少\n");
			CASE FileNameTooLong : printf("文件名过长\n");
			CASE FileOpenFail : printf("文件打开失败\n");
			CASE InputErr : printf("参数输入错误\n");
			CASE FileDataErr : printf("文件数据错误\n");
			CASE SerialPortNameErr : printf("串口名错误\n");
			CASE UnknownBlueTooth : printf("未知型号蓝牙\n");
		}
	}
}

int main(int argc, char *argv[])
{
	static const char *cmd[] = { "-H", "-R", "-A", "-P" };
	static App const operat[] = { HTforCMD, RDforCMD, SetATforCMD, PIDtest };
	
	if (argc > 1)
	{
		int countCmd;
		for (countCmd = 0; countCmd < sizeof(operat) / sizeof(operat[0]); countCmd++)
		{
			if (STRCMP(argv[1], == , cmd[countCmd]))
			{
				operat[countCmd](argc, argv);
				DisplayError();
				return EXIT_SUCCESS;
			}
		}
		if (countCmd == sizeof(operat) / sizeof(operat[0]))
		{
			printf("未知参数:%s\n", argv[1]);
		}
	}
	else
	{
		printf("没有参数传入\n");
	}
	return EXIT_SUCCESS;
}