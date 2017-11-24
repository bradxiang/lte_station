#include "StdAfx.h"
//#include "MillerRabin.h"
#include "MyRSA.h"


//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CRSA::CRSA(void)
{
	m_lp=0;m_lq=0;
	m_ln=0;m_ld=0;m_le;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CRSA::~CRSA(void)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：Miller-Rabin素性检验算法
//------------------------------------------------------------------------------------------------------
BOOL CRSA::TestPrime(long n)
{
// 	if(miller_rabin(n,6))
// 		return TRUE;
// 	else
 		return FALSE;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：模重复平方法求模幂
//------------------------------------------------------------------------------------------------------
long CRSA::powmod(long a, long b, long n)
{
	long A=1;
	while(b!=0)
	{
		if(b%2) A=(A*a)%n;
		a=(a*a)%n;
		b=b>>1;
	}	
	return A;	
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：加密函数
//------------------------------------------------------------------------------------------------------
long CRSA::Encrypt(long msg)
{
	return powmod(msg,m_le,m_ln);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：解密一个数字
//------------------------------------------------------------------------------------------------------
long CRSA:: Decrypt(long enmsg)
{
	return powmod(enmsg,m_ld,m_ln);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：解密一个数字加密密钥
//------------------------------------------------------------------------------------------------------
BOOL CRSA::SetEncrypteParam(long e,long n)
{
	if(n>MAXN)
	{
		AfxMessageBox("N过大，超出计算范围");
		return FALSE;
	}
	m_le=e;
	m_ln=n;
	return TRUE;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：加密一个字符串
//------------------------------------------------------------------------------------------------------
CString CRSA:: EncryptStr(CString msg)
{
	char buff[MAXLENGTH];
	long enbuff[MAXLENGTH];
	strcpy(buff,(LPCSTR)msg);
	for(int i=0;i<msg.GetLength();i++)
		enbuff[i]=Encrypt((long)buff[i]);
	//把long按字节转化为string
	u_char longbuff[INTERlEN];
	CString v_sEnstr="",v_stemp;
	for(int i=0;i<msg.GetLength();i++)
	{
		memcpy(longbuff,&enbuff[i],INTERlEN);
		for (int j=0;j<CODELEN;j++)
		{
			v_stemp.Format("%X",longbuff[j]);
			if(v_stemp.GetLength()<2)
				v_stemp="0"+v_stemp;
			v_sEnstr+=v_stemp;
		}
	}
	return v_sEnstr;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：设定解密密钥
//------------------------------------------------------------------------------------------------------
BOOL CRSA:: SetDecrypteParam(long d,long n)
{
	if(n>MAXN)
	{
		AfxMessageBox("N过大，超出计算能力");
		return FALSE;
	}
	m_ld=d;
	m_ln=n;
	return TRUE;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：解密一个字符串
//------------------------------------------------------------------------------------------------------
CString CRSA:: DecryptStr(CString enmsg)
{
	enmsg.Trim();
	if(enmsg.GetLength()%CODELEN!=0)
	{
		AfxMessageBox("密文不完整");
		return "";
	}
	if(enmsg.GetLength()>1000*CODELEN)
	{
		AfxMessageBox("密文超过最大长度");
		return "";
	}
	//转化为long型数组
	int v_iCharCount=enmsg.GetLength()/CODELEN;							//获取明文长度
	u_char buff[INTERlEN];                                              //一个long型的内存空间
	long longbuff[MAXLENGTH];
	CString temp;
	for (int i=0;i<v_iCharCount;i++)
	{
		memset(buff,0,INTERlEN);
		for (int j=0;j<CODELEN;j++)
		{
			temp=enmsg.Left(2);
			enmsg.Delete(0,2);
			buff[j]=string2u_char(temp);
		}
		memcpy(&longbuff[i],buff,INTERlEN);                              //把4个字节转化为一个long型	
	}
	//解密
	CString v_sDeStr="";
	char v_chDeBuff[MAXLENGTH+1];
	long v_lDecTemp;
	for (int i=0;i<v_iCharCount;i++)
	{
		v_lDecTemp=Decrypt(longbuff[i]);
		v_chDeBuff[i]=(u_char)v_lDecTemp;
	}
	v_chDeBuff[v_iCharCount]='\0';
	v_sDeStr=v_chDeBuff;
	return v_sDeStr;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取随机的参数
//------------------------------------------------------------------------------------------------------
BOOL CRSA:: GetRandomParam()
{
	long temp;
	srand((unsigned)time(NULL));//初始化随机数生成器
	m_lp=GetPrime(2147483647);
	m_lq=GetPrime(2147483647);
	m_ln=m_lp*m_lq;
	if(m_ln>MAXN)
	{
		AfxMessageBox("N过大,请重新计算");
		return FALSE;
	}
	temp=(m_lp-1)*(m_lq-1);
	m_ld=GetPrime(temp);
	m_le=Invmod(m_ld,temp);
	return true;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取所有参数
//------------------------------------------------------------------------------------------------------
void CRSA::GetAllParam(long &p,long &q,long &n,long &d,long &e)
{
	p=m_lp;q=m_lq;n=m_ln;d=m_ld;e=m_le;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：根据pq获得个参数
//------------------------------------------------------------------------------------------------------
BOOL CRSA:: GetParByPQ(long p,long q)
{
	if(!TestPrime(p))
	{
		AfxMessageBox("P 为非素数");
		return FALSE;
	}
	if(!TestPrime(q))
	{
		AfxMessageBox("Q 为非素数");
		return FALSE;
	}
	long temp;
	srand((unsigned)time(NULL));//初始化随机数生成器
	m_lp=p;
	m_lq=q;
	m_ln=m_lp*m_lq;
	if(m_ln>MAXN)
	{
		AfxMessageBox("N过大,请重新选择p和q的值");
		return FALSE;
	}
	temp=(m_lp-1)*(m_lq-1);
	m_ld=GetPrime(temp);
	m_le=Invmod(m_ld,temp);
	return TRUE;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：调用素性检验算法获得不超过N的一个随机数
//------------------------------------------------------------------------------------------------------
long CRSA:: GetPrime(long N)
{
	long n;
	for(;;)
	{
		n=random(3,N);
		if(TestPrime(n)&&n>9999) return n;
		//Sleep(50);
	}
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：扩展的欧几里德算法求模逆元
//------------------------------------------------------------------------------------------------------
long CRSA::Invmod(long a, long b)
{
	long s0=1,s1=0,s2,q,t,b0;
	b0=b;
	while(b)
	{
		q=a/b;
		s2=s0-q*s1;
		if(s2>=0)
			s2=s2%b0;
		else
			s2=b0-(-s2)%b0;
		s0=s1;s1=s2;
		t=b;b=a%b;a=t;
	}
	if(a==1) return s0;
	else return 0;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：字符转换
//------------------------------------------------------------------------------------------------------
u_char CRSA::string2u_char(CString Num)
{
	//CString temp;
	u_char v_chResult;
	//temp=Num.Left(1);
	v_chResult=string2u_char_Harf(Num.Left(1))<<4;
	v_chResult+=string2u_char_Harf(Num.Right(1));
	return v_chResult;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：字符转换
//------------------------------------------------------------------------------------------------------
u_char CRSA::string2u_char_Harf(CString Num)
{
	char temp[2],v_chResult=0x00;
	strcpy( (char*)temp,(LPCSTR)Num);	
	if (0x30<=temp[0]&&temp[0]<=0x39)
	{
		v_chResult=(u_char)atoi(Num);
	}
	else if (0x41<=temp[0]&&temp[0]<=0x46)
	{
		v_chResult=(u_char)temp[0]-0x37;
	}
	else if (0x61<=temp[0]&&temp[0]<=0x66)
	{
		v_chResult=(u_char)temp[0]-0x57;
	}
	return v_chResult;
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
