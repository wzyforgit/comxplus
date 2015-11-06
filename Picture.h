#ifndef PICTURE_H
#define PICTURE_H

#define PIDW 1024
#define PIDH 768

typedef struct StepResponseData
{
	int Data[PIDW / 2];	//�ļ��ڲ�����
	int DataNum;		//��������
	int MaxData;		//�������ֵ
	int td;				//�ӳ�ʱ��
	int tr;				//����ʱ��
	int tp;				//��ֵʱ��
	int ts;				//����ʱ��
	int MaxOutput;		//��������
	int base;			//����ֵ
	int T;				//��������
	double Overshoot;	//������
}StepResponseData;

void CreatPIDPicture(int Data[], int DataNum, int Base, int max);

#endif