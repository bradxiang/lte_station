#pragma once
//#include "StdAfx.h"
#include   <afxmt.h> 

const double pi = 3.14159265358979324;
const double a = 6378245.0;
const double ee = 0.00669342162296594323;
const  double x_pi = 3.14159265358979324 * 3000.0 / 180.0;

class  CMyMap_Baidu 
{
public:
	CMyMap_Baidu(void);
	~CMyMap_Baidu(void);
public:

	//坐标转换：经纬度==〉百度座标
	static void		transform2Mars(double wgLat, double wgLon,double &mgLat,double &mgLon);
	static void		bd_encrypt(double gg_lat, double gg_lon,double &bd_lat,double & bd_lon);
	static bool		outOfChina(double lat, double lon);
	static double	transformLat(double x, double y);
	static double	transformLon(double x, double y);

	//坐标转换：百度座标==〉经纬度
	static void		transform2bd(double &v_dsw_lng, double &v_dsw_lat,double &v_dne_lng,double &v_dne_lat);
};

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

//------------------------------------------------------------------------------------------------------
//	函数功能：坐标系
//------------------------------------------------------------------------------------------------------
class CEarthPoint
{
public:
	double            m_dEarthR;        //    6371.004    6378.140    6356.755
	double            m_dRealR;
	double            m_dAngE;			//    纬度-东
	double            m_dAngN;			//    经度-北
	double            m_x;
	double            m_y;
	double            m_z;
public:
	CEarthPoint()
	{
		m_dEarthR = 6371.004;
		m_dRealR = 0.0;
	}
	virtual ~CEarthPoint()
	{
	}
public:
	//    dAngN:    纬度, 北纬正数, 南纬负数
	//    dAngE:    经度, 东经正数, 西经负数
	BOOL    SetPos(double dAngN, double dAngE)
	{
		double        dPlanR;
		double        dEarthR;

		m_dAngE        = dAngE;
		m_dAngN        = dAngN;
		Revisory(dAngN);
		dEarthR = m_dRealR;

		dAngE = m_dAngE * M_PI / 180;
		dAngN = m_dAngN * M_PI / 180;

		m_z = dEarthR * sin(dAngN);
		dPlanR = dEarthR * cos(dAngN);
		m_x = dPlanR * cos(dAngE);
		m_y = dPlanR * sin(dAngE);
		return TRUE;
	}


	BOOL    Revisory(double dAngN)
	{
		double        dMin, dMax, dDlt;

		dAngN = fabs(dAngN);
		// 		if(dAngN > 90.0)
		// 			assert(0);
		dMin = 6356.755;
		dMax = 6378.140;
		dDlt = (dMax - dMin) * dAngN / 90;
		m_dRealR = dMax-dDlt;
		return TRUE;
	}

	BOOL    SetPos(double x, double y, double z)
	{
		return FALSE;
	}


	static    double    GetArcDistance(double dDis, double dRealR)
	{
		double            dRealDis, dAng;

		dAng = asin(dDis/2/dRealR);
		dAng *= 2;
		dRealDis = dRealR * dAng;
		return dRealDis;
	}

	static    double    Distance(CEarthPoint &Pos1, CEarthPoint &Pos2, BOOL bArc = TRUE)
	{
		double            dDis1, dSum, dRealDis;
		double            dRealR;

		dSum = 0;
		dDis1 = Pos1.m_x - Pos2.m_x;    dSum += dDis1 * dDis1;
		dDis1 = Pos1.m_y - Pos2.m_y;    dSum += dDis1 * dDis1;
		dDis1 = Pos1.m_z - Pos2.m_z;    dSum += dDis1 * dDis1;
		dSum = sqrt(dSum);
		if(bArc == FALSE)
		{
			dRealDis = dSum;
			return dRealDis;
		}

		dRealR = (Pos1.m_dRealR + Pos2.m_dRealR)/2;
		dRealDis = GetArcDistance(dSum, dRealR);
		return dRealDis;
	}
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
