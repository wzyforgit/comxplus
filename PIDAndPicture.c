#define _CRT_SECURE_NO_WARNINGS 1
#include"App.h"
#include"Picture.h"
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<windows.h>

static BOOL GetSRDdata(FILE *ReFile, char *BasePar,StepResponseData *SRD)
{
	SRD->td = SRD->tr = SRD->tp = SRD->ts = 0;
	SRD->Overshoot = 0;

	if (sscanf(BasePar, "%d-%d-%d", &SRD->MaxOutput, &SRD->base, &SRD->T) < 3)
	{
		AErr = InputErr;
		return FALSE;
	}

	int ch;
	for (SRD->DataNum = 0, SRD->MaxData = 0; SRD->DataNum < PIDW / 2; SRD->DataNum++)
	{
		/*数据读取*/
		if (fscanf(ReFile, "%d", &SRD->Data[SRD->DataNum])<1)
		{
			AErr = FileDataErr;
			return FALSE;
		}

		/*计算超调量,峰值时间*/
		if (SRD->Data[SRD->DataNum]>SRD->MaxData)
		{
			SRD->MaxData = SRD->Data[SRD->DataNum];
			if (SRD->MaxData > SRD->base)
			{
				SRD->tp = (SRD->DataNum - 1) * SRD->T;
				SRD->Overshoot = (SRD->MaxData - SRD->base) / (double)SRD->base;
			}
		}

		/*计算调节时间*/
		if (SRD->ts == 0 && abs(SRD->Data[SRD->DataNum] - SRD->base) / (double)SRD->base <= 0.02)
		{
			SRD->ts = (SRD->DataNum - 1) * SRD->T;
		}
		if (SRD->ts != 0 && abs(SRD->Data[SRD->DataNum] - SRD->base) / (double)SRD->base > 0.02)
		{
			SRD->ts = 0;
		}

		/*计算延迟时间*/
		if (SRD->td == 0 && SRD->Data[SRD->DataNum] >= SRD->base / 2)
		{
			SRD->td = (SRD->DataNum - 1) * SRD->T;
		}

		/*计算上升时间*/
		if (SRD->tr == 0 && SRD->Data[SRD->DataNum] >= SRD->base)
		{
			SRD->tr = (SRD->DataNum - 1) * SRD->T;
		}

		/*读取段终值符(根据通信协议设计)及判断EOF*/
		ch = getc(ReFile);
		ch = getc(ReFile);
		if (ch == EOF)
			break;
		else
			ungetc(ch, ReFile);
	}
	return TRUE;
}

void PIDtest(int argc,char **argv)
{
	if (argc < 4)
	{
		AErr = FewParameter;
		return;
	}

	if (strlen(argv[2])>MAX_PATH)
	{
		AErr = FileNameTooLong;
		return;
	}

	FILE *ReFile;
	ReFile = fopen(argv[2], "r");
	if (ReFile == NULL)
	{
		AErr = FileOpenFail;
		return;
	}

	StepResponseData SRD;
	if (GetSRDdata(ReFile, argv[3], &SRD) == FALSE)
	{
		return;
	}

	printf("td=%dms\ntr=%dms\ntp=%dms\nts=%dms\nσ=%g\n", SRD.td, SRD.tr, SRD.tp, SRD.ts, SRD.Overshoot);
	printf("按任意键开始创建本次测试的阶跃响应图像\n");
	_getch();

	printf("正在生成图片...\n");
	CreatPIDPicture(SRD.Data, SRD.DataNum, SRD.base, SRD.MaxOutput);
}