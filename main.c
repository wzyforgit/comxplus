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
			CASE Err_OpenCom : printf("���ڴ�ʧ��\n");
			CASE Err_SetTimeOuts : printf("���ó�ʱʧ��\n");
			CASE Err_SetBuffer : printf("���û�����ʧ��\n");
			CASE Err_GetDCB : printf("��ȡDCBʧ��\n");
			CASE Err_SetDCB : printf("����DCBʧ��\n");
			CASE Err_ClearBuf : printf("���������ʧ��\n");
			CASE Err_PullOut : printf("���ڱ��γ�\n");
		}
	}

	if (AErr != NoErr)
	{
		switch (AErr)
		{
			CASE FewParameter : printf("��������\n");
			CASE FileNameTooLong : printf("�ļ�������\n");
			CASE FileOpenFail : printf("�ļ���ʧ��\n");
			CASE InputErr : printf("�����������\n");
			CASE FileDataErr : printf("�ļ����ݴ���\n");
			CASE SerialPortNameErr : printf("����������\n");
			CASE UnknownBlueTooth : printf("δ֪�ͺ�����\n");
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
			printf("δ֪����:%s\n", argv[1]);
		}
	}
	else
	{
		printf("û�в�������\n");
	}
	return EXIT_SUCCESS;
}