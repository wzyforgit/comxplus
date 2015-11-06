#ifndef KUROKO_H
#define KUROKO_H

#define _CRT_SECURE_NO_WARNINGS 1

#include<windows.h>

#define CASE break;case
#define STRCMP(a,R,b) (strcmp(a,b) R 0)
#define CheckComHandle(Hcom) {if (Hcom == NULL)return;}

extern volatile BOOL fExit;

enum k_Error
{
	NoErr = 0,
	Err_OpenCom,
	Err_SetTimeOuts,
	Err_SetBuffer,
	Err_GetDCB,
	Err_SetDCB,
	Err_ClearBuf,
	Err_PullOut
};
extern enum k_Error Err;

typedef struct SerialPort
{
	HANDLE Hcom;
	char ComName[7];
	DWORD BaudRate;
	BYTE ByteSize;
	BYTE Parity;
	BYTE StopBits;
}SerialPort;

/*公共内核函数*/
extern BOOL OpenCom(HANDLE *Hcom, char *ComName, DWORD DesAcc, DWORD Flags);
extern BOOL SetBufAndTimeOut(HANDLE Hcom);
extern BOOL SetDcb(HANDLE Hcom, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits);
extern BOOL ClearBuffer(HANDLE Hcom);
extern BOOL SeMessege(HANDLE Hcom, char *SeBuf);
extern BOOL ReMessege(HANDLE Hcom, char *ReBuf);
extern HANDLE SerialPortInit(SerialPort SP);
extern unsigned int WINAPI ExitMonitor(PVOID PM);

#endif