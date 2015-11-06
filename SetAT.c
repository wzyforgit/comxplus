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
	printf("请选择指令:\n");
	printf("1.设置名字\t\t2.设置密码\n"\
		"3.设为主机\t\t4.设为从机\n"\
		"5.设为任意连接\t\t6.设置通信参数\n"\
		"7.清空配对列表\t\t8.测试AT模式\n"\
		"9.退出AT模式并复位蓝牙模块\n"\
		"显示OK表示设置成功,并可以继续发送指令\n设置失败将显示00,此时建议退出程序\n");
}

static char* SetPra05(int Choose)
{
	static char Buf[50];
	switch (Choose)
	{
		CASE NAME : sprintf(Buf, "AT+NAME="); printf("请输入新名字(enter结束)\n");
		CASE PSWD : sprintf(Buf, "AT+PSWD="); printf("请输入新密码(enter结束)\n");
		CASE UART : sprintf(Buf, "AT+UART="); printf("请依照文档格式输入新传输模式(enter结束)\n");
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
		default:printf("指令错误!\n"); continue;
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