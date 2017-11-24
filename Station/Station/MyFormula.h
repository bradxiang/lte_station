#pragma once

#define MAXOP 1000
#define NUMBER '0'
#define MAXVAL 100
#define BUFSIZE 1000

class CMyFormula
{
public:
	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	CMyFormula(void);													//���캯��
	~CMyFormula(void);													//��������


	//���㹫ʽ-----------------------------------------------------------------------------------------------
	struct PM_Formula_Struct											//���㹫ʽ�ṹ
	{
		CString		v_sPM_Formula;										//�����/����ֵ
		bool		v_bFormula;											//1������ֵ��		0���������
	};
	typedef list<PM_Formula_Struct> mPM_Formula;						//������mPM_Formula

	//�Զ���
	void	replacesign(char *os,char *ts);								//�滻���ʽ�е��ض�����
	float	simcalc(char *os);											//�����ַ��ͱ��ʽ
	float	calc(int sign, float op1, float op2);
	int		cmp(int s1, int s2);
	void	push_d(float f);
	float	pop_d(void);
	void	push_s(int c);
	int		pop_s(void);
	int		getop(char *os,char *s);
	int		getch(char *os);
	void	ungetch(int c);
	int		isdigit( int ch );

	//��չ����ʽ���������������
	void	My_Get_Formula(CString v_sFormula);							//�ֽ⹫ʽ���ʽ			

	mPM_Formula				v_mFormula;									//��ʽ�ֽ����������ã��������������
	mPM_Formula::iterator	v_pFormula;									//������

	//------------------------------------------------------------------------------------------------------
	//	��������
	//------------------------------------------------------------------------------------------------------
	int		sp_d;
	float	val_d[MAXVAL];

	/* a stack for sign */
	int		sp_s;
	int		val_s[MAXVAL];


	char	buf[BUFSIZE];
	int		bufp;
	int		j;
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
