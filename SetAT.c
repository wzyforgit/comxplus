#include"Kuroko.h"
#include"App.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

enum SetPre
{
	NAME = 1, PSWD, UART
};

static void Menu(void)
{
	printf("��ѡ��ָ��:\n");
	printf("1.��������\t\t2.��������\n"\
		"3.��Ϊ����\t\t4.��Ϊ�ӻ�\n"\
		"5.��Ϊ��������\t\t6.����ͨ�Ų���\n"\
		"7.�������б�\t\t8.����ATģʽ\n"\
		"9.�˳�ATģʽ����λ����ģ��\n"\
		"��ʾOK��ʾ���óɹ�,�����Լ�������ָ��\n����ʧ�ܽ���ʾ00,��ʱ�����˳�����\n");
}

static char* SetPra05(int Choose)
{
	static char Buf[50];
	switch (Choose)
	{
		CASE NAME : sprintf(Buf, "AT+NAME="); printf("������������(enter����)\n");
		CASE PSWD : sprintf(Buf, "AT+PSWD="); printf("������������(enter����)\n");
		CASE UART : sprintf(Buf, "AT+UART="); printf("�������ĵ���ʽ�����´���ģʽ(enter����)\n");
	}
	scanf("%41s", &Buf[8]);
	int NameLen;
	NameLen = strlen(Buf);
	NameLen == 49 ? sprintf(&Buf[47], "\r\n") :
		sprintf(&Buf[strlen(Buf)], "\r\n");
	return Buf;
}

static void Set05AT(SerialPort SP)
{
	SP.Hcom = SerialPortInit(SP);
	CheckComHandle(SP.Hcom);
	Menu();
	char *Order;
	int Choose = 0;
	char Buf[3];
	Buf[2] = '\0';
	memset(Buf, '0', 2);
	while (Choose != '9')
	{
		Choose = _getch();
		switch (Choose)
		{
		default:printf("ָ�����!\n"); continue;
			CASE '1':Order = SetPra05(NAME);
			CASE '2':Order = SetPra05(PSWD);
			CASE '3':Order = "AT+ROLE=0\r\n";
			CASE '4':Order = "AT+ROLE=1\r\n";
			CASE '5':Order = "AT+CMODE=0\r\n";
			CASE '6':Order = SetPra05(UART);
			CASE '7':Order = "AT+RMAAD\r\n";
			CASE '8':Order = "AT\r\n";
			CASE '9':Order = "AT+RESET\r\n";
		}
		if (!SeMessege(SP.Hcom, Order))
		{
			return;
		}
		Sleep(500);
		ReMessege(SP.Hcom, &Buf[0]);
		ReMessege(SP.Hcom, &Buf[1]);
		printf(Buf);
		memset(Buf, '0', 2);
	}
	CloseHandle(SP.Hcom);
}

void SetATforCMD(int argc, char **argv)
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

	if (STRCMP(argv[3], == , "05"))
	{
		SP.BaudRate = 38400;
		SP.ByteSize = 8;
		SP.Parity = NOPARITY;
		SP.StopBits = ONESTOPBIT;
	}
	else
	{
		AErr = UnknownBlueTooth;
		return;
	}

	Set05AT(SP);
}