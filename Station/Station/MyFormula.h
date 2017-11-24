#pragma once

#define MAXOP 1000
#define NUMBER '0'
#define MAXVAL 100
#define BUFSIZE 1000

class CMyFormula
{
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义
	//------------------------------------------------------------------------------------------------------
	CMyFormula(void);													//构造函数
	~CMyFormula(void);													//析构函数


	//计算公式-----------------------------------------------------------------------------------------------
	struct PM_Formula_Struct											//计算公式结构
	{
		CString		v_sPM_Formula;										//运算符/运算值
		bool		v_bFormula;											//1：运算值；		0：运算符；
	};
	typedef list<PM_Formula_Struct> mPM_Formula;						//容器：mPM_Formula

	//自定义
	void	replacesign(char *os,char *ts);								//替换表达式中的特定变量
	float	simcalc(char *os);											//计算字符型表达式
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

	//扩展：公式计算外的其它功能
	void	My_Get_Formula(CString v_sFormula);							//分解公式表达式			

	mPM_Formula				v_mFormula;									//公式分解容器【放置：运算符、变量】
	mPM_Formula::iterator	v_pFormula;									//迭代器

	//------------------------------------------------------------------------------------------------------
	//	变量定义
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
