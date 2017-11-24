#include "StdAfx.h"
#include "MyMap_Baidu.h"


//------------------------------------------------------------------------------------------------------			
//	函数功能：构造函数
//------------------------------------------------------------------------------------------------------
CMyMap_Baidu::CMyMap_Baidu(void)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：析构函数
//------------------------------------------------------------------------------------------------------
CMyMap_Baidu::~CMyMap_Baidu(void)
{
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：百度座标==〉经纬度
//------------------------------------------------------------------------------------------------------
void CMyMap_Baidu::transform2bd(double &v_dsw_lng, double &v_dsw_lat,double &v_dne_lng,double &v_dne_lat)
{
	double x_pi=3.1415926;
	double bd_x=v_dsw_lng - 0.0065;
	double bd_y=v_dsw_lat - 0.006;
	double z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	double theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dsw_lng = z * cos(theta)-0.005;
	v_dsw_lat = z * sin(theta)-0.001;

	bd_x=v_dne_lng - 0.0065;
	bd_y=v_dne_lat - 0.006;
	z = sqrt(bd_x * bd_x + bd_y * bd_y) - 0.00002 * sin(bd_y * x_pi);
	theta = atan2(bd_y, bd_x) - 0.000003 * cos(bd_x * x_pi);
	v_dne_lng = z * cos(theta)-0.005;
	v_dne_lat = z * sin(theta)-0.001;
}

//------------------------------------------------------------------------------------------------------			
//	地球坐标转换为火星坐标
//	World Geodetic System ==> Mars Geodetic System
//	@param wgLat  地球坐标
//	@param wgLon
//	mglat,mglon 火星坐标
//------------------------------------------------------------------------------------------------------
void CMyMap_Baidu::transform2Mars(double wgLat, double wgLon,double &mgLat,double &mgLon)
{
	if (outOfChina(wgLat, wgLon))
	{
		mgLat  = wgLat;
        mgLon = wgLon;
		return ;
	}
	double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
    double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
	double radLat = wgLat / 180.0 * pi;
	double magic = sin(radLat);
	magic = 1 - ee * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
	dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
	mgLat = wgLat + dLat;
	mgLon = wgLon + dLon;
}

//------------------------------------------------------------------------------------------------------			
//	火星坐标转换为百度坐标
//	@param gg_lat
//	@param gg_lon
//------------------------------------------------------------------------------------------------------			
void CMyMap_Baidu::bd_encrypt(double gg_lat, double gg_lon,double &bd_lat,double & bd_lon)
{
	double x = gg_lon, y = gg_lat;
	double z = sqrt(x * x + y * y) + 0.00002 * sin(y * x_pi);
	double theta = atan2(y, x) + 0.000003 * cos(x * x_pi);
	bd_lon = z * cos(theta) + 0.0065;
	bd_lat = z * sin(theta) + 0.006;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：
//------------------------------------------------------------------------------------------------------			
bool CMyMap_Baidu::outOfChina(double lat, double lon)
{
	if (lon < 72.004 || lon > 137.8347)
		return true;
	if (lat < 0.8293 || lat > 55.8271)
		return true;
	return false;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：
//------------------------------------------------------------------------------------------------------			
double CMyMap_Baidu::transformLat(double x, double y)
{
	double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
	ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
	return ret;
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：
//------------------------------------------------------------------------------------------------------			
double CMyMap_Baidu::transformLon(double x, double y)
{
	double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
	ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
	return ret;
}

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------