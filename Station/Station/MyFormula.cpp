#include "StdAfx.h"
#include "MyFormula.h"
//#include "RT-SPAA.h"


//------------------------------------------------------------------------------------------------------
//	构造函数
//------------------------------------------------------------------------------------------------------
CMyFormula::CMyFormula(void)
{
}

//------------------------------------------------------------------------------------------------------
//	析构函数
//------------------------------------------------------------------------------------------------------
CMyFormula::~CMyFormula(void)
{
	v_mFormula.clear();													//容器清除
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
void CMyFormula::replacesign(char *os,char *ts)//替换表达式中的特定变量
{
	char temp[5][32]={};
	char m[32]={"  0.57000"};//M值的替换输入值
	char v[5][32] = {"  0.65000","1.5","3.2","8.5","5.4"};//V的替换值
	int vx[9];

	int i,j,k,vn;
	i = 0;
	j = 0;
	k = 0;

	while(os[i] != '\0'){
		if (os[i] != 'M')
			temp[j][k++] = os[i++];
		else{
			j++;
			k = 0;
			i++;
		}
	}
	strcpy(ts,temp[0]);//cstrcpy,cstrcat....
	if (j > 0){
		strcat(ts,m);//ftoa?
		strcat(ts,temp[1]);
	}// 完成对m值的替换

	strcpy(os,ts);//cstrcpy,cstrcat....

	i = 0;
	j = 0;
	k = 0;
	vn = 0; //记录vx个数
	memset(temp,0,sizeof(temp));

	while(os[i] != '\0'){
		if (os[i] != 'V')
			temp[j][k++] = os[i++];
		else{
			j++;
			k = 0;
			i++;
			vx[vn++] = os[i++] - '0';

		}
	}
	strcpy(ts,temp[0]);//cstrcpy,cstrcat....
	if (j > 0){
		for( i = 1; i < j + 1; i ++){
			strcat(ts,v[vx[i-1]-1]);//ftoa?
			strcat(ts,temp[i]);
		}
	}// 完成对m值的替换

//	printf("The final formula is %s\n",ts);
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
float CMyFormula::simcalc(char *os)//计算字符型表达式
{
	int type, sign;
	float op1, op2;
	char s[MAXOP];

	sp_d = 0;
	/* a stack for sign */
	sp_s = 0;
	bufp = 0;
	j = 0;

	while ((type = getop(os,s)) != EOF) {//Ctrl+Z可以输入EOF
		switch (type) {
				case NUMBER:
					push_d(atof(s));
					break;
				case '(':
					push_s(type);
					break;
				case '+':
				case '-':
				case '*':
				case '/':
					//				case '^':
					while (cmp(sign = pop_s(),type) >= 0) {
						op2 = pop_d();
						op1 = pop_d();
						push_d(calc(sign, op1, op2));
					}
					push_s(sign);
					push_s(type);
					break;
				case ')':
					while((sign = pop_s()) != '(') {
						op2 = pop_d();
						op1 = pop_d();
						push_d(calc(sign, op1, op2));
					}
					break;

				case '=':
					break;
				case '\0':
					while((sign = pop_s()) != EOF) {
						op2 = pop_d();
						op1 = pop_d();
						push_d(calc(sign, op1, op2));
					}
					return  pop_d();
		}
	}

}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
float CMyFormula::calc(int sign, float op1, float op2)
{
	switch (sign) {
		case '+':
			return op1 + op2;
			break;
		case '*':
			return op1 * op2;
			break;
		case '-':
			return op1 - op2;
			break;
		case '/':
			if (op2 != 0)
				return op1 / op2;
			else
				printf("error: zero divisor\n");
			break;
			//       case '^':
			//               return pow(op1, op2);
			//               break;
		default:
			break;
	}
	return 0.0;
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
int CMyFormula::cmp(int s1, int s2)
{
	int s[2],i;

	s[0] = s1;
	s[1] = s2;
	for (i = 0; i < 2; i++) {
		switch (s[i]) {
				case EOF:
					s[i] = 0;
					break;
				case '(':
					s[i] = 0;
					break;
				case '+':
					s[i] = 1;
					break;
				case '-':
					s[i] = 1;
					break;
				case '*':
					s[i] = 2;
					break;
				case '/':
					s[i] = 2;
					break;
					//               case '^':
					//                       s[i] = 3;
					//                       break;
				default:
					printf("error:unknown comand\n");
					return 10;
		}
	}
	return s[0] - s[1];
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
void CMyFormula::push_d(float f)
{
	if (sp_d < MAXVAL)
		val_d[sp_d++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
float CMyFormula::pop_d(void)
{
	if (sp_d > 0)
		return val_d[--sp_d];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
void CMyFormula::push_s(int c)
{
	if (sp_s < MAXVAL)
		val_s[sp_s++] = c;
	else
		printf("error: stack full, can't push %c\n", c);
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
int CMyFormula::pop_s(void)
{
	if (sp_s > 0)
		return val_s[--sp_s];
	else 
		return EOF;
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
int CMyFormula::getop(char *os,char *s)
{
	int i, c;

	while ((s[0] = c = getch(os)) == ' ' || c == '\t');
	s[1] = '\0';
	if (!isdigit(c) && c != '.')
		return c;
	i = 0;
	if (isdigit(c))
		while (isdigit(s[++i] = c = getch(os)));
	if (c == '.')
		while (isdigit(s[++i] = c = getch(os)));
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
int CMyFormula::getch(char *os)
{
	return (bufp > 0) ? buf[--bufp] : os[j++];
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
void CMyFormula::ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

//------------------------------------------------------------------------------------------------------
//	功能函数：
//------------------------------------------------------------------------------------------------------
int CMyFormula::isdigit( int ch )

{
	return (unsigned int)(ch - '0') < 10u;

}//判断字符ch是否为数字


//======================================================================================================
//	扩展：公式计算外的其它功能
//		  (1) 分解公式表达式
//		  (2)
//		  (3)
//======================================================================================================


//------------------------------------------------------------------------------------------------------			
//	函数功能：分解公式表达式
//	参	  数：v_sFormula：计算公式；	v_mFormula：计算分解保存容器；【运算符,0】【运算值,1】
//	工	  作：基于性能计算公式，计算性能；
//	计算公式分解：(A1+A2)/(A3+A4) 放入容器 ( A1 + A2 )/( A3 + A4 ) ，分为运算符，运算值，运算值用于获取具体数值
//------------------------------------------------------------------------------------------------------
void CMyFormula::My_Get_Formula(CString v_sFormula)
{
	CString		v_sStr="";												//临时变量
	bool		v_bFlag=false;											//标记：运算符
	char*		v_pFormula;												//临时变量
	PM_Formula_Struct	v_cPM_Formula;									//结构

	v_pFormula=v_sFormula.GetBuffer(0);
	for (int vi=0;vi<v_sFormula.GetLength();vi++)						//遍历计算公式
	{
		if ((unsigned char)v_pFormula[vi]<0x30 && (unsigned char)v_pFormula[vi]!=0x2E)	//运算符？？？
		{
			if (v_bFlag)
			{
				if (!v_sStr.IsEmpty())
				{
					v_cPM_Formula.v_sPM_Formula=v_sStr;
					v_cPM_Formula.v_bFormula=v_bFlag;
					if (v_sStr.Left(1)<="9")							//若预算值的第一位必须为字符，否则为常数，如：100等，按运算符处理【即不用获取Counter值】
						v_cPM_Formula.v_bFormula=false;					//运算符
					v_mFormula.push_back(v_cPM_Formula);				//增加：运算值
					v_sStr="";											//复位
				}
				v_bFlag=false;											//标记：运算符
			}
		}
		else
		{
			if (!v_bFlag)
			{
				if (!v_sStr.IsEmpty())
				{
					v_cPM_Formula.v_sPM_Formula=v_sStr;
					v_cPM_Formula.v_bFormula=v_bFlag;
					v_mFormula.push_back(v_cPM_Formula);				//增加：运算符
					v_sStr="";											//复位
				}
				v_bFlag=true;											//标记：运算值
			}
		}
		v_sStr+=v_sFormula.Mid(vi,1);									//获取：字符串
	}
	if (!v_sStr.IsEmpty())
	{
		v_cPM_Formula.v_sPM_Formula=v_sStr;
		v_cPM_Formula.v_bFormula=v_bFlag;
		if(v_cPM_Formula.v_bFormula)									//运算值？？？
		{
			if (v_sStr.Left(1)<="9")									//若预算值的第一位必须为字符，否则为常数，如：100等，按运算符处理【即不用获取Counter值】
				v_cPM_Formula.v_bFormula=false;							//运算符
		}
		v_mFormula.push_back(v_cPM_Formula);							//增加：运算值/运算符
	}
}

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
