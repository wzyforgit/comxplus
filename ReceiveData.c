#include"Kuroko.h"
#include"App.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<process.h>

static void ReceiveData(char *FileName ,SerialPort SP)
{
	SP.Hcom = SerialPortInit(SP);
	CheckComHandle(SP.Hcom);

	/*׼���洢�ļ�*/
	FILE *pSave;
	if (strlen(FileName) > MAX_PATH)
	{
		AErr = FileNameTooLong;
		return;
	}
	pSave = fopen(FileName, "w");
	if (pSave == NULL)
	{
		AErr = FileOpenFail;
		return;
	}

	/*��������*/
	HANDLE Wait;
	Wait = (HANDLE)_beginthreadex(NULL, 0, ExitMonitor, NULL, 0, NULL);

	/*����������ʼ��,����λ������λ��ͳһ*/
	const char *DataStart = "tsD";
	/*����������ֹ��,����λ������λ��ͳһ*/
	const char *DataEnd = "deD";
	/*�ν�����,����λ������λ��ͳһ*/
	const char SectionEnd = '#';

	char SEbuf[4];
	memset(SEbuf, 0, sizeof(char)* 4);
	while (1)
	{
		if (!ReMessege(SP.Hcom, &SEbuf[0]))
		{
			if (Err == Err_PullOut)
			{
				return;
			}
		}
		else
		{
			if (STRCMP(SEbuf, == , DataStart))
				break;
			SEbuf[2] = SEbuf[1];
			SEbuf[1] = SEbuf[0];
		}
	}

	printf("��⵽������ʼ��,��ʼ�����������ļ�\n");
	memset(SEbuf, 0, sizeof(char)* 3);
	while (1)
	{
		if (ReMessege(SP.Hcom, &SEbuf[0]))
		{
			fprintf(pSave, "%c", SEbuf[0]);
			if (SEbuf[0] == SectionEnd&&fExit == 1)
			{
				printf("ͬʱ��⵽����ֹ�����˳�\n");
				break;
			}
			if (STRCMP(SEbuf, == , DataEnd))
			{
				printf("��⵽������ֹ��\n");
				break;
			}
			SEbuf[2] = SEbuf[1];
			SEbuf[1] = SEbuf[0];
		}
		else
		{
			if (Err == Err_PullOut)
			{
				return;
			}
		}
	}

	CloseHandle(Wait);
	CloseHandle(SP.Hcom);
	fclose(pSave);
	fExit = 0;
}

void RDforCMD(int argc, char **argv)
{
	if (argc < 5)
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

	ReceiveData(argv[4], SP);
}