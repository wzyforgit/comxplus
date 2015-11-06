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

	/*准备存储文件*/
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

	/*传输数据*/
	HANDLE Wait;
	Wait = (HANDLE)_beginthreadex(NULL, 0, ExitMonitor, NULL, 0, NULL);

	/*传输数据起始符,由上位机和下位机统一*/
	const char *DataStart = "tsD";
	/*传输数据终止符,由上位机和下位机统一*/
	const char *DataEnd = "deD";
	/*段结束符,由上位机和下位机统一*/
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

	printf("检测到数据起始符,开始保存数据至文件\n");
	memset(SEbuf, 0, sizeof(char)* 3);
	while (1)
	{
		if (ReMessege(SP.Hcom, &SEbuf[0]))
		{
			fprintf(pSave, "%c", SEbuf[0]);
			if (SEbuf[0] == SectionEnd&&fExit == 1)
			{
				printf("同时检测到段终止符和退出\n");
				break;
			}
			if (STRCMP(SEbuf, == , DataEnd))
			{
				printf("检测到数据终止符\n");
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