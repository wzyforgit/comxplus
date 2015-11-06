#ifndef APP_H
#define APP_H

#define CASE break;case
#define STRCMP(a,R,b) (strcmp(a,b) R 0)
typedef void(*App)(int, char**);

enum A_Error
{
	None=0,
	FewParameter,
	FileNameTooLong,
	FileOpenFail,
	InputErr,
	FileDataErr,
	SerialPortNameErr,
	UnknownBlueTooth,
};
extern enum A_Error AErr;

#ifdef KUROKO_H

extern void DealStdStr(const char *string, SerialPort *SP);

#endif

extern void HTforCMD(int argc, char **argv);
extern void RDforCMD(int argc, char **argv);
extern void SetATforCMD(int argc, char **argv);
extern void PIDtest(int argc, char **argv);

#endif