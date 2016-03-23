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
		ComName,//�����߼���
		DesAcc,//��������,GENERIC_READΪ�ɶ�,GENERIC_WRITEΪ��д
		0,//�Ƿ����Թ���,��Ϊ���ڲ��ɹ���,����Ϊ0
		NULL,//���ð�ȫ���Խṹ,ȱʡΪNULL
		OPEN_EXISTING,//������־,�Դ��ڱ���ΪOPEN_EXISTING(�򿪶����Ǵ���)
		Flags,//�Ƿ������첽����,0Ϊͬ��,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPEDΪ�첽
		NULL//ѡ��ģ���ļ�,�Դ��ڱ���ΪNULL
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
	/*���ô���*/
	//����I/O������(����ͨ�����ʵ���)
	if (SetupComm(Hcom,//�豸����
		1024,//���뻺������С(�ֽ�)
		1024 //������������С(�ֽ�)
		) == FALSE)
	{
		Err = Err_SetBuffer;
		return FALSE;
	}
	//�������ó�ʱ�Ľṹ��  ע:��ʱ���������������õĳ�ʱʱ����δ���ɶ�/д����,��/д�����Ի�����
	COMMTIMEOUTS TimeOuts;
	/*
	1.��/д�ܳ�ʱ=ϵ��*�ַ���+����
	2.ϵ���볣����Ϊ0����ʾ��ʹ�øó�ʱ
	3.����������ʱΪMAXDWORD�Ҷ�ϵ���ͳ�����Ϊ0,��ô�������ͻ���������,�����Ƿ�����Ҫ�����ַ�
	4.�첽ͨ��ʱ,��/д���ܻ������ɲ���ǰ����,��ʱ��ʱ�涨���ǲ���������ʱ��
	5.ʱ�䵥λ��Ϊms
	*/
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = MAXDWORD;//��������ʱ
	TimeOuts.ReadTotalTimeoutMultiplier = 0;//��ʱ��ϵ��
	TimeOuts.ReadTotalTimeoutConstant = 1;//��ʱ�䳣��
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 50;//дʱ��ϵ��
	TimeOuts.WriteTotalTimeoutConstant = 1000;//дʱ�䳣��
	//д�볬ʱ����,��ȡ��ǰ��ʱ����ʹ��GetCommTimeouts()
	if (SetCommTimeouts(Hcom, &TimeOuts) == FALSE)
	{
		Err = Err_SetTimeOuts;
		return FALSE;
	}
	return TRUE;
}

BOOL SetDcb(HANDLE Hcom, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits)
{
	//��������
	DCB dcb;
	//��ȡ��ǰDCB��Ϣ
	if (GetCommState(Hcom, &dcb) == FALSE)
	{
		Err = Err_GetDCB;
		return FALSE;
	}
	dcb.BaudRate = BaudRate;//���ò�����,��ʽΪCBR_xxx
	dcb.ByteSize = ByteSize;//�ֽڳ���(4-8)
	dcb.Parity = Parity;//��żУ�鷽��,ODDPARITY ��У�� EVENPARITY żУ�� MARKPARITY ����У�� NOPARITY ��У��
	dcb.StopBits = StopBits;//ֹͣλλ��,ONESTOPBIT 1λֹͣλ TWOSTOPBITS 2λֹͣλ ONE5STOPBITS 1.5λֹͣλ
	if (Parity == NOPARITY)
		dcb.fParity = 0;
	else
		dcb.fParity = 1;
	//д��DCB��Ϣ
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
	/*�򿪴���*/
	if (OpenCom(&Hcom, SP.ComName, GENERIC_READ | GENERIC_WRITE, 0) == FALSE)
	{
		return NULL;
	}
	/*���û������볬ʱ*/
	if (SetBufAndTimeOut(Hcom) == FALSE)
	{
		return NULL;
	}
	/*����DCB*/
	if (SetDcb(Hcom, SP.BaudRate, SP.ByteSize, SP.Parity, SP.StopBits) == FALSE)
	{
		return NULL;
	}
	/*����������*/
	if (PurgeComm(Hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE)
	{
		return NULL;
	}
	return Hcom;
}