#include"Kuroko.h"
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<process.h>
#include<windows.h>

volatile BOOL fExit = 0;
enum k_Error Err = NoErr;

unsigned int WINAPI ExitMonitor(PVOID PM)
{
	int x;
	while (1)
	{
		x = _getch();
		if (x == 27)
		{
			fExit = 1;
			break;
		}
	}
	return 0;
}

BOOL SeMessege(HANDLE Hcom, char *SeBuf)
{
	DWORD WriteNum;
	if (WriteFile(Hcom, SeBuf, strlen(SeBuf), &WriteNum, NULL) == FALSE)
	{
		Err = Err_PullOut;
		return FALSE;
	}
	return TRUE;
}

BOOL ReMessege(HANDLE Hcom, char *ReBuf)
{
	DWORD ReadNum;
	if (ReadFile(Hcom, ReBuf, 1, &ReadNum, NULL) == FALSE)
	{
		Err = Err_PullOut;
		return FALSE;
	}
	if (ReadNum != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL OpenCom(HANDLE *Hcom, char *ComName, DWORD DesAcc, DWORD Flags)
{
	*Hcom = CreateFileA(
		ComName,//串口逻辑名
		DesAcc,//访问类型,GENERIC_READ为可读,GENERIC_WRITE为可写
		0,//是否可以共享,因为串口不可共享,必须为0
		NULL,//引用安全属性结构,缺省为NULL
		OPEN_EXISTING,//创建标志,对串口必须为OPEN_EXISTING(打开而不是创建)
		Flags,//是否进行异步操作,0为同步,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED为异步
		NULL//选用模板文件,对串口必须为NULL
		);
	if (*Hcom == (HANDLE)-1)
	{
		Err = Err_OpenCom;
		return FALSE;
	}
	return TRUE;
}

BOOL SetBufAndTimeOut(HANDLE Hcom)
{
	/*配置串口*/
	//设置I/O缓冲区(根据通信速率调整)
	if (SetupComm(Hcom,//设备句柄
		1024,//输入缓冲区大小(字节)
		1024 //输出缓冲区大小(字节)
		) == FALSE)
	{
		Err = Err_SetBuffer;
		return FALSE;
	}
	//定义设置超时的结构体  注:超时的作用是若在设置的超时时间内未完成读/写操作,读/写操作仍会结束
	COMMTIMEOUTS TimeOuts;
	/*
	1.读/写总超时=系数*字符数+常量
	2.系数与常量均为0则表示不使用该超时
	3.若读间隔超时为MAXDWORD且读系数和常量均为0,那么读入后就会立即返回,不管是否读入要求的字符
	4.异步通信时,读/写可能会在完成操作前返回,此时超时规定的是操作的完成时间
	5.时间单位均为ms
	*/
	//设定读超时
	TimeOuts.ReadIntervalTimeout = MAXDWORD;//读间隔超时
	TimeOuts.ReadTotalTimeoutMultiplier = 1;//读时间系数
	TimeOuts.ReadTotalTimeoutConstant = 10;//读时间常量
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 50;//写时间系数
	TimeOuts.WriteTotalTimeoutConstant = 1000;//写时间常量
	//写入超时设置,读取当前超时设置使用GetCommTimeouts()
	if (SetCommTimeouts(Hcom, &TimeOuts) == FALSE)
	{
		Err = Err_SetTimeOuts;
		return FALSE;
	}
	return TRUE;
}

BOOL SetDcb(HANDLE Hcom, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits)
{
	//传输设置
	DCB dcb;
	//获取当前DCB信息
	if (GetCommState(Hcom, &dcb) == FALSE)
	{
		Err = Err_GetDCB;
		return FALSE;
	}
	dcb.BaudRate = BaudRate;//设置波特率,格式为CBR_xxx
	dcb.ByteSize = ByteSize;//字节长度(4-8)
	dcb.Parity = Parity;//奇偶校验方法,ODDPARITY 奇校验 EVENPARITY 偶校验 MARKPARITY 标记校验 NOPARITY 无校验
	dcb.StopBits = StopBits;//停止位位数,ONESTOPBIT 1位停止位 TWOSTOPBITS 2位停止位 ONE5STOPBITS 1.5位停止位
	if (Parity == NOPARITY)
		dcb.fParity = 0;
	else
		dcb.fParity = 1;
	//写入DCB信息
	if (SetCommState(Hcom, &dcb) == FALSE)
	{
		Err = Err_SetDCB;
		return FALSE;
	}
	return TRUE;
}

BOOL ClearBuffer(HANDLE Hcom)
{
	if (PurgeComm(Hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
	{
		Err = Err_ClearBuf;
		return FALSE;
	}
	return TRUE;
}

HANDLE SerialPortInit(SerialPort SP)
{
	HANDLE Hcom = NULL;
	/*打开串口*/
	if (OpenCom(&Hcom, SP.ComName, GENERIC_READ | GENERIC_WRITE, 0) == FALSE)
	{
		return NULL;
	}
	/*设置缓冲区与超时*/
	if (SetBufAndTimeOut(Hcom) == FALSE)
	{
		return NULL;
	}
	/*设置DCB*/
	if (SetDcb(Hcom, SP.BaudRate, SP.ByteSize, SP.Parity, SP.StopBits) == FALSE)
	{
		return NULL;
	}
	/*清理缓冲区*/
	if (PurgeComm(Hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
	{
		return NULL;
	}
	return Hcom;
}