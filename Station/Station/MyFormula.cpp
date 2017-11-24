#include "StdAfx.h"
#include "MyFormula.h"
//#include "RT-SPAA.h"


//------------------------------------------------------------------------------------------------------
//	���캯��
//------------------------------------------------------------------------------------------------------
CMyFormula::CMyFormula(void)
{
}

//------------------------------------------------------------------------------------------------------
//	��������
//------------------------------------------------------------------------------------------------------
CMyFormula::~CMyFormula(void)
{
	v_mFormula.clear();													//�������
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
void CMyFormula::replacesign(char *os,char *ts)//�滻���ʽ�е��ض�����
{
	char temp[5][32]={};
	char m[32]={"  0.57000"};//Mֵ���滻����ֵ
	char v[5][32] = {"  0.65000","1.5","3.2","8.5","5.4"};//V���滻ֵ
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
	}// ��ɶ�mֵ���滻

	strcpy(os,ts);//cstrcpy,cstrcat....

	i = 0;
	j = 0;
	k = 0;
	vn = 0; //��¼vx����
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
	}// ��ɶ�mֵ���滻

//	printf("The final formula is %s\n",ts);
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
float CMyFormula::simcalc(char *os)//�����ַ��ͱ��ʽ
{
	int type, sign;
	float op1, op2;
	char s[MAXOP];

	sp_d = 0;
	/* a stack for sign */
	sp_s = 0;
	bufp = 0;
	j = 0;

	while ((type = getop(os,s)) != EOF) {//Ctrl+Z��������EOF
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
//	���ܺ�����
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
//	���ܺ�����
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
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
void CMyFormula::push_d(float f)
{
	if (sp_d < MAXVAL)
		val_d[sp_d++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
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
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
void CMyFormula::push_s(int c)
{
	if (sp_s < MAXVAL)
		val_s[sp_s++] = c;
	else
		printf("error: stack full, can't push %c\n", c);
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
int CMyFormula::pop_s(void)
{
	if (sp_s > 0)
		return val_s[--sp_s];
	else 
		return EOF;
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
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
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
int CMyFormula::getch(char *os)
{
	return (bufp > 0) ? buf[--bufp] : os[j++];
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
void CMyFormula::ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

//------------------------------------------------------------------------------------------------------
//	���ܺ�����
//------------------------------------------------------------------------------------------------------
int CMyFormula::isdigit( int ch )

{
	return (unsigned int)(ch - '0') < 10u;

}//�ж��ַ�ch�Ƿ�Ϊ����


//======================================================================================================
//	��չ����ʽ���������������
//		  (1) �ֽ⹫ʽ���ʽ
//		  (2)
//		  (3)
//======================================================================================================


//------------------------------------------------------------------------------------------------------			
//	�������ܣ��ֽ⹫ʽ���ʽ
//	��	  ����v_sFormula�����㹫ʽ��	v_mFormula������ֽⱣ���������������,0��������ֵ,1��
//	��	  �����������ܼ��㹫ʽ���������ܣ�
//	���㹫ʽ�ֽ⣺(A1+A2)/(A3+A4) �������� ( A1 + A2 )/( A3 + A4 ) ����Ϊ�����������ֵ������ֵ���ڻ�ȡ������ֵ
//------------------------------------------------------------------------------------------------------
void CMyFormula::My_Get_Formula(CString v_sFormula)
{
	CString		v_sStr="";												//��ʱ����
	bool		v_bFlag=false;											//��ǣ������
	char*		v_pFormula;												//��ʱ����
	PM_Formula_Struct	v_cPM_Formula;									//�ṹ

	v_pFormula=v_sFormula.GetBuffer(0);
	for (int vi=0;vi<v_sFormula.GetLength();vi++)						//�������㹫ʽ
	{
		if ((unsigned char)v_pFormula[vi]<0x30 && (unsigned char)v_pFormula[vi]!=0x2E)	//�����������
		{
			if (v_bFlag)
			{
				if (!v_sStr.IsEmpty())
				{
					v_cPM_Formula.v_sPM_Formula=v_sStr;
					v_cPM_Formula.v_bFormula=v_bFlag;
					if (v_sStr.Left(1)<="9")							//��Ԥ��ֵ�ĵ�һλ����Ϊ�ַ�������Ϊ�������磺100�ȣ�����������������û�ȡCounterֵ��
						v_cPM_Formula.v_bFormula=false;					//�����
					v_mFormula.push_back(v_cPM_Formula);				//���ӣ�����ֵ
					v_sStr="";											//��λ
				}
				v_bFlag=false;											//��ǣ������
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
					v_mFormula.push_back(v_cPM_Formula);				//���ӣ������
					v_sStr="";											//��λ
				}
				v_bFlag=true;											//��ǣ�����ֵ
			}
		}
		v_sStr+=v_sFormula.Mid(vi,1);									//��ȡ���ַ���
	}
	if (!v_sStr.IsEmpty())
	{
		v_cPM_Formula.v_sPM_Formula=v_sStr;
		v_cPM_Formula.v_bFormula=v_bFlag;
		if(v_cPM_Formula.v_bFormula)									//����ֵ������
		{
			if (v_sStr.Left(1)<="9")									//��Ԥ��ֵ�ĵ�һλ����Ϊ�ַ�������Ϊ�������磺100�ȣ�����������������û�ȡCounterֵ��
				v_cPM_Formula.v_bFormula=false;							//�����
		}
		v_mFormula.push_back(v_cPM_Formula);							//���ӣ�����ֵ/�����
	}
}

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
