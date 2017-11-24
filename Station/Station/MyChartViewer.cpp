// CMyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyChartViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	类定义：CMyChartViewer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//	构造函数
//-------------------------------------------------------------------------------------------------
CMyChartViewer::CMyChartViewer()
{
	v_dTime_Stamps=NULL;
	v_iTime_Sum=1440;													//默认：粒度总数【全部数据总数量】
}

//-------------------------------------------------------------------------------------------------
//	析构函数
//-------------------------------------------------------------------------------------------------
CMyChartViewer::~CMyChartViewer()
{
	mChart_Data::iterator v_pIter;
	for(v_pIter=v_mChart_Data.begin();v_pIter!=v_mChart_Data.end();v_pIter++)	//清除
		delete[] v_pIter->second.v_dChart_Value;
	v_mChart_Data.clear();												//容器清除
	v_mTime_Array.clear();												//容器清除
	if (v_dTime_Stamps!=NULL)
		delete[] v_dTime_Stamps;										//清除数据
}

//------------------------------------------------------------------------------------------------------
//	函数功能：绘图Chart
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::drawChart(CChartViewer *viewer)
{
	CString		v_sTemp,v_Name;											//临时变量
	int			startIndex;												//变量：绘图开始点位置【时间点】
	int			endIndex;												//变量：绘图结束点位置【时间点】
	int			noOfPoints;												//变量：绘图点总数【时间点】
	DoubleArray viewPortDataSeriesA;									//绘图数据数组

	//【1】绘图时间窗获取：绘图的时间粒度范围【开始粒度(位置)、结束粒度(位置)】
	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());								//计算：开始时间位置【绘图点】
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());	//计算：结束时间位置【绘图点】

	// Get the array indexes that corresponds to the visible start and end dates
	startIndex = (int)floor(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Show), viewPortStartDate));	//计算：绘图开始点位置【时间点】
	endIndex = (int)ceil(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Show), viewPortEndDate));		//计算：绘图结束点位置【时间点】
	noOfPoints = endIndex - startIndex + 1;																	//计算：绘图点总数【时间点】
	
	// Extract the part of the data array that are visible.
	DoubleArray viewPortTimeStamps = DoubleArray(DoubleArray(v_dTime_Stamps,v_iTime_Show).data + startIndex, noOfPoints);	//获取：绘图X轴时间粒度序号
 
	//【2】图形区域设置
	int v_iX_A=v_cUI_Data.v_iChart_X-v_cUI_Data.v_iChart_X0-10;			//X轴宽度	[10：右边宽度]
	int v_iY_A=v_cUI_Data.v_iChart_Y-v_cUI_Data.v_iChart_Y0-12;			//Y轴宽度	[10：低边宽度]

	// Create an XYChart object (v_iX_A x v_iY_A) pixels in size, with light grey (f4f4f4) 
	// background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
    XYChart *c = new XYChart(v_iX_A, v_iY_A, 0xf4f4f4, 0x000000, 1);	//Chart生成[v_iX_A,v_iY_A]
    c->setRoundedFrame(v_iBgColor);										//背景色
    
    // Set the plotarea at (55, 55) and of size (v_iX_A-20,v_iY_A-20) pixels. Use white (ffffff) 
    // background. Enable both horizontal and vertical grids by setting their colors to 
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	int	v_iY2ax=0;	
//	if (v_cUI_Data.v_iDen || v_cUI_Data.v_iNum)							//分母或分子绘图？？？
//		v_iY2ax=60;														//右边：空出60画Y2轴
	int	v_iY_A_Time=0;	
    c->setPlotArea(65,80, v_iX_A-90-v_iY2ax, v_iY_A-125-v_iY_A_Time, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);		//绘图区域定义
    c->setClipping();

	// Add a legend box at (50, 28) using horizontal layout. Use 10pts Arial Bold as font, with
	// transparent background.
	if (v_mChart_Data.size()<8)											//曲线数量小于8
		c->addLegend(50, 28, false, "simkai.ttf", 10)->setBackground(Chart::Transparent);	//图例绘制


    // Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
    // grey (dddddd) background, black (000000) border, and a glass like raised effect.
	c->addTitle(TCHARtoUTF8(_T(v_cUI_Data.v_sKPI_Name)),"simkai.ttf", 16)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());	//Chart题目：KPI_Name
            
    // Set the reference font size of the legend box
    c->getLegend()->setFontSize(8);

    //Y轴名称
	if (v_cUI_Data.v_iTime==0)											//比率
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Data")), "arialbd.ttf", 10);
	else																//时天周月
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Data")), "arialbd.ttf", 10);		//Y轴名称

	// set the axis, label and title colors for the primary y axis to green
	// (0x008000) to match the second data set
	c->yAxis2()->setColors(0x008000, 0x008000, 0x008000);

	// Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
    // 15  pixels between minor ticks. This shows more minor grid lines on the chart.
    c->xAxis()->setTickDensity(75, 15);

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

	//【3】X轴数据显示
	// Add a line layer for the lines, using a line width of 2 pixels
	LineLayer *layer = c->addLineLayer();
	layer->setLineWidth(2);
	// In this demo, we do not have too many data points. In real code, the chart may contain a lot
	// of data points when fully zoomed out - much more than the number of horizontal pixels in this
	// plot area. So it is a good idea to use fast line mode.
	layer->setFastLineMode();

	//数据装载：X轴
    layer->setXData(viewPortTimeStamps);								//赋值：X轴时间轴数据【显示的时间信息】

	//【4】绘制曲线 【从容器中(v_mChart_Data)获取数据，绘图。】
	int						v_iLine_Num=0;								//线序号	[设置线颜色时使用]
	int						v_iNum=-1;									//初始值：曲线计数器
	int						v_iColor=0x0000FF;							//初始值：颜色值
	CString					v_sInstance_Flag = "";						//名称：默认为空
	mChart_Data::iterator	v_pIterator_KPI;							//v_mChart_Data的map容器迭代器
	//绘图数据遍历【曲线(多条)数据在容器v_mChart_Data】
	v_pIterator_KPI=v_mChart_Data.begin();								//赋值：头指针
	while(v_pIterator_KPI != v_mChart_Data.end())						//曲线(多条)遍历结束？？？
	{
		if (!v_cUI_Data.v_iDay)											//无效：单天？？？
		{
			//多天绘制
			v_iNum++;													//曲线计数器(绘制颜色控制)
			v_iColor=My_GetChart_Color(v_iNum);							//计算：曲线颜色(依据曲线计数器)

			//获取：绘图数据【放入：viewPortDataSeriesA】
			viewPortDataSeriesA =DoubleArray(DoubleArray(v_pIterator_KPI->second.v_dChart_Value,v_iTime_Show).data + startIndex, noOfPoints);		//数据拷贝【绘图数据放入绘图数据数组viewPortDataSeriesA】

			//赋值：绘图缓冲区【来源：viewPortDataSeriesA】
			if (v_cUI_Data.v_iPoint)									//有效：绘图数据点标志【显示：数据+名称】
				layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)))->setDataSymbol(Chart::CircleSymbol, 5,0xff0000,0xff0000);	//加入一条曲线：数据+名称
			else														//无效：绘图数据点标志【只显示数据】
				layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)));															//加入一条曲线：数据
			v_pIterator_KPI->second.v_iColor=v_iColor;					//保存：绘图颜色

			//缺点绘图：(1)不画线；(2)画虚线；   [下面的语句有效则画虚线，无效则不画线]
			layer->setGapColor(c->dashLineColor(Chart::SameAsMainColor));	//缺点绘图：线型及颜色
		}
		else															//单天
		{
			//单天绘制【把多天数据，在一天粒度上绘制，用于比较每天变化的区别】
			COleDateTime	v_dDate=v_cUI_Data.v_dDate_Start;			//查询开始日期
			for (int vi=0;vi<v_cUI_Data.v_iChart_Day_Sum;vi++)			//有效天数
			{
				//曲线数量计算器原则(单天且多线：曲线名称不同计数；其它情况：每次计数；)
				if (v_cUI_Data.v_iMuti_Line)							//单天 且 多线 ？？？
				{
					if (v_sInstance_Flag!=v_pIterator_KPI->second.v_sName)	//判断：连续两个曲线名称不相同？？？【去掉名称最后的6位(即日期)】
					{
						v_sInstance_Flag=v_pIterator_KPI->second.v_sName;
						v_iNum++;										//曲线计数器
					}
				}
				else
					v_iNum++;											//曲线计数器
				v_iColor=My_GetChart_Color(v_iNum);						//计算：曲线颜色(依据曲线计数器)

				//获取：绘图数据【放入：viewPortDataSeriesA】
				viewPortDataSeriesA =DoubleArray(DoubleArray(v_pIterator_KPI->second.v_dChart_Value,v_iTime_Show).data + v_iTime_Show*vi+startIndex, noOfPoints);		//数据拷贝【绘图数据放入绘图数据数组viewPortDataSeriesA】

				//赋值：绘图缓冲区【来源：viewPortDataSeriesA】
				v_sTemp=v_dDate.Format("_%m-%d");						//日期
				v_Name = v_pIterator_KPI->second.v_sName + v_sTemp;		//曲线名称+日期
				if (v_cUI_Data.v_iPoint)								//有效：绘图数据点标志【显示：数据+名称】
					layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_Name)))->setDataSymbol(Chart::CircleSymbol, 5,0xff0000,0xff0000);	//加入一条曲线：数据+名称
				else													//无效：绘图数据点标志【只显示数据】
					layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_Name)));															//加入一条曲线：数据
				v_pIterator_KPI->second.v_iColor=v_iColor;				//保存：绘图颜色

				//缺点绘图：(1)不画线；(2)画虚线；   [下面的语句有效则画虚线，无效则不画线]
				layer->setGapColor(c->dashLineColor(Chart::SameAsMainColor));	//缺点绘图：线型及颜色

				v_dDate += COleDateTimeSpan(1,0,0,0);					//增加一天
			}
		}
		v_pIterator_KPI++;												//迭代器加一
	}

	//【5】图形参数设置：Configure axis scale and labelling
	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	viewer->syncDateAxisWithViewPort("x", c->xAxis());					//设置：时间轴

	// If all ticks are yearly aligned, then we use "yyyy" as the label format.
	c->xAxis()->setFormatCondition("align", 360 * 86400);
	c->xAxis()->setLabelFormat("{value|yyyy}");

	//设置Y轴刻度
// 	if (v_cUI_Data.v_iY_scale)
// 	{
// 		CString v_sMax,v_sMin;
// 		c->yAxis()->setDateScale(v_cUI_Data.v_iMin,v_cUI_Data.v_iMax,0,0);		//设置：Y轴刻度
// 	}
// 	else
// 		c->yAxis()->setDateScale();										//设置：Y轴自动刻度
// 	c->yAxis2()->setDateScale();										//设置：Y2轴自动刻度

	// If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first 
	// label of a year, and "mmm" for other labels.
	c->xAxis()->setFormatCondition("align", 30 * 86400);
	c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|yyyy-mm}", 
		Chart::AllPassFilter(), "{value|yyyy-mm}");

	// If all ticks are daily algined, then we use "mmm dd<*br*>yyyy" in bold font as the 
	// first label of a year, and "mmm dd" in bold font as the first label of a month, and
	// "dd" for other labels.
	c->xAxis()->setFormatCondition("align", 86400);
	c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(),"<*block,halign=left*><*font=bold*>{value|yy-mm-dd}", 
		Chart::StartOfMonthFilter(), "<*font=bold*>{value|yy-mm-dd}");
	c->xAxis()->setMultiFormat(Chart::AllPassFilter(), "{value|yy-mm-dd}");

	// For all other cases (sub-daily ticks), use "hh:nn<*br*>mmm dd" for the first label of
	// a day, and "hh:nn" for other labels.
	c->xAxis()->setFormatCondition("else");
	c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(),"<*font=bold*>{value|dd_hh:nn}", Chart::AllPassFilter(), "{value|dd_hh:nn}");

	// Include track line with legend. If the mouse is on the plot area, show the track 
    // line with legend at the mouse position; otherwise, show them for the latest data
    // values (that is, at the rightmost position).
//	trackLineLegend(c, viewer->isMouseOnPlotArea() ? viewer->getPlotAreaMouseX() : c->getPlotArea()->getRightX());

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
    viewer->setChart(c);
}

//------------------------------------------------------------------------------------------------------
//	函数功能：绘图Chart  [Bar]
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::drawChart_Bar(CChartViewer *viewer)
{
	int	startIndex;														//加入：实际数据点数
	int endIndex;														//加入：实际数据点数
	int noOfPoints;														//

	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());

	// Get the array indexes that corresponds to the visible start and end dates
	startIndex = (int)floor(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Sum), viewPortStartDate));	//加入：实际数据点数
	endIndex = (int)ceil(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Sum), viewPortEndDate));			//加入：实际数据点数
	noOfPoints = endIndex - startIndex + 1;

	// Extract the part of the data array that are visible.
	DoubleArray viewPortTimeStamps = DoubleArray(DoubleArray(v_dTime_Stamps,v_iTime_Sum).data + startIndex, noOfPoints);

	int v_iX_A=v_cUI_Data.v_iChart_X-v_cUI_Data.v_iChart_X0-10;			//X轴宽度	[10：右边宽度]
	int v_iY_A=v_cUI_Data.v_iChart_Y-v_cUI_Data.v_iChart_Y0-12;			//Y轴宽度	[10：低边宽度]

	// Create an XYChart object (v_iX_A x v_iY_A) pixels in size, with light grey (f4f4f4) 
	// background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
	XYChart *c = new XYChart(v_iX_A, v_iY_A, 0xf4f4f4, 0x000000, 1);	//Chart生成[v_iX_A,v_iY_A]
	c->setRoundedFrame(v_iBgColor);										//背景色

	// Set the plotarea at (55, 55) and of size (v_iX_A-20,v_iY_A-20) pixels. Use white (ffffff) 
	// background. Enable both horizontal and vertical grids by setting their colors to 
	// grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	int	v_iY2ax=0;	
	//	if (v_cUI_Data.v_iDen || v_cUI_Data.v_iNum)							//分母或分子绘图？？？
	//		v_iY2ax=60;														//右边：空出60画Y2轴
	int	v_iY_A_Time=0;	
	c->setPlotArea(65,80, v_iX_A-90-v_iY2ax, v_iY_A-125-v_iY_A_Time, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);		//绘图区域定义
	c->setClipping();

	// Add a legend box at (50, 28) using horizontal layout. Use 10pts Arial Bold as font, with
	// transparent background.
	if (v_mChart_Data.size()<8)											//曲线数量小于8
		c->addLegend(50, 28, false, "simkai.ttf", 10)->setBackground(Chart::Transparent);	//图例绘制


	// Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
	// grey (dddddd) background, black (000000) border, and a glass like raised effect.
	c->addTitle(TCHARtoUTF8(_T(v_cUI_Data.v_sKPI_Name)),"simkai.ttf", 16)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());	//Chart题目：KPI_Name

	// Set the reference font size of the legend box
	c->getLegend()->setFontSize(8);

	//Y轴名称
	if (v_cUI_Data.v_iTime==0)											//AP数量
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Amounts")), "arialbd.ttf", 10);
	else																//时天周月
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Capacity (GBytes)")), "arialbd.ttf", 10);		//Y轴名称

	// set the axis, label and title colors for the primary y axis to green
	// (0x008000) to match the second data set
	c->yAxis2()->setColors(0x008000, 0x008000, 0x008000);

	// Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
	// 15  pixels between minor ticks. This shows more minor grid lines on the chart.
	c->xAxis()->setTickDensity(75, 15);

	// Set the axes width to 2 pixels
	c->xAxis()->setWidth(2);
	c->yAxis()->setWidth(2);

	///////////////////////////////////////////////////////////////////////////////////////
	// Add data to chart
	///////////////////////////////////////////////////////////////////////////////////////
	// Add a line layer for the lines, using a line width of 2 pixels
	BarLayer *layer = c->addBarLayer(Chart::Side, 4);
	
	if (v_cUI_Data.v_iBar_3d)											//3D有效？？？
		layer->set3D(10);												//设置：3D参数

	//数据装载：X轴
	layer->setXData(viewPortTimeStamps);								//X轴：时间轴

	int	v_iLine_Num=0;													//线序号	[设置线颜色时使用]
	// 数据装载：曲线
	int		v_iNum=-1;													//颜色序号：初始值
	mChart_Data::iterator v_pIterator_KPI;								//KPI_Instance的map容器迭代器
	v_pIterator_KPI=v_mChart_Data.begin();								//从头开始
	int	v_iColor=0x0000FF;												//颜色值
	CString	v_sInstance_Flag="";										//名称：默认为空
	while(v_pIterator_KPI != v_mChart_Data.end())						//结束？？？
	{
		if (v_cUI_Data.v_iDay && v_cUI_Data.v_iMuti_Line)
		{
			CString	v_sTemp=v_pIterator_KPI->second.v_sName;
			if (v_sInstance_Flag!=v_sTemp.Left(v_sTemp.GetLength()-6))
			{
				v_sInstance_Flag=v_sTemp.Left(v_sTemp.GetLength()-6);
				v_iNum++;												//颜色序号
			}
		}
		else
			v_iNum++;													//颜色序号

		//KPI_Value
		DoubleArray viewPortDataSeriesA,viewPortDataSeriesB;			//绘图数组
		v_iColor=My_GetChart_Color(v_iNum);								//任意颜色
		viewPortDataSeriesA =DoubleArray(DoubleArray(v_pIterator_KPI->second.v_dChart_Value,v_iTime_Sum).data + startIndex, noOfPoints);		//数据拷贝

		if (v_cUI_Data.v_iPoint)										//有效：绘图数据点标志
			layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)))->setDataSymbol(Chart::CircleSymbol, 5,0xff0000,0xff0000);		//加入一条曲线：数据+名称
		else
			layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)));																//加入一条曲线：数据+名称	

		switch(v_cUI_Data.v_iBar_Type)									//Bar类型
		{
			case 0:														//Bar: 方形
				layer->setBarShape(Chart::SquareShape);					//默认
				break;
			case 1:														//Bar: 圆形
				layer->setBarShape(Chart::CircleShape);
				break;
			case 2:														//Bar: 星形
				layer->setBarShape(Chart::StarShape(5));
				break;
			case 3:														//Bar: 菱形
				layer->setBarShape(Chart::PolygonShape(6));
				break;
		}

		v_pIterator_KPI->second.v_iColor=v_iColor;						//保存：绘图颜色
		//缺点绘图：(1)不画线；(2)画虚线；   [下面的语句有效则画虚线，无效则不画线]
		//		layer->setGapColor(c->dashLineColor(Chart::SameAsMainColor));	//缺点绘图：线型及颜色

		v_pIterator_KPI++;												//迭代器加一
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// Configure axis scale and labelling
	///////////////////////////////////////////////////////////////////////////////////////
	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	viewer->syncDateAxisWithViewPort("x", c->xAxis());					//设置：时间轴

	// If all ticks are yearly aligned, then we use "yyyy" as the label format.
	c->xAxis()->setFormatCondition("align", 360 * 86400);
	c->xAxis()->setLabelFormat("{value|yyyy}");

	//设置Y轴刻度
// 	if (v_cUI_Data.v_iY_scale)
// 	{
// 		CString v_sMax,v_sMin;
// 		c->yAxis()->setDateScale(v_cUI_Data.v_iMin,v_cUI_Data.v_iMax,0,0);		//设置：Y轴刻度
// 	}
// 	else
// 		c->yAxis()->setDateScale();										//设置：Y轴自动刻度
// 	c->yAxis2()->setDateScale();										//设置：Y2轴自动刻度

	// If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first 
	// label of a year, and "mmm" for other labels.
	c->xAxis()->setFormatCondition("align", 30 * 86400);
	c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|yyyy-mm}", 
		Chart::AllPassFilter(), "{value|yyyy-mm}");

	// If all ticks are daily algined, then we use "mmm dd<*br*>yyyy" in bold font as the 
	// first label of a year, and "mmm dd" in bold font as the first label of a month, and
	// "dd" for other labels.
	c->xAxis()->setFormatCondition("align", 86400);
	c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(),"<*block,halign=left*><*font=bold*>{value|yy-mm-dd}", 
		Chart::StartOfMonthFilter(), "<*font=bold*>{value|yy-mm-dd}");
	c->xAxis()->setMultiFormat(Chart::AllPassFilter(), "{value|yy-mm-dd}");

	// For all other cases (sub-daily ticks), use "hh:nn<*br*>mmm dd" for the first label of
	// a day, and "hh:nn" for other labels.
	c->xAxis()->setFormatCondition("else");
	c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(),"<*font=bold*>{value|dd_hh:nn}", Chart::AllPassFilter(), "{value|dd_hh:nn}");

	// Include track line with legend. If the mouse is on the plot area, show the track 
	// line with legend at the mouse position; otherwise, show them for the latest data
	// values (that is, at the rightmost position).
	//	trackLineLegend(c, viewer->isMouseOnPlotArea() ? viewer->getPlotAreaMouseX() : c->getPlotArea()->getRightX());

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(c);
}

//------------------------------------------------------------------------------------------------------
//	函数功能：Initialize the CChartViewer
//	工    作：【1】数据初始化；【2】时间轴数据初始化；【3】初始化：图形参数；
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::initChartViewer(CChartViewer *viewer)
{
	int			v_iTime_Sec;											//临时变量
	double		v_dNextDataTime;										//时间轴

	//【1】数据初始化
	mChart_Data::iterator v_pIter;
	for(v_pIter=v_mChart_Data.begin();v_pIter!=v_mChart_Data.end();v_pIter++)	//清除
		delete[] v_pIter->second.v_dChart_Value;
	v_mChart_Data.clear();												//容器复位【数据容器(名称、数据、颜色)】
	v_mTime_Array.clear();												//容器复位【时间与粒度序号】

	//【2】时间轴数据初始化【计算：粒度总数、X轴时间数组】
	v_dNextDataTime = Chart::chartTime(v_cUI_Data.v_dDate_Start.GetYear(),v_cUI_Data.v_dDate_Start.GetMonth(),v_cUI_Data.v_dDate_Start.GetDay(),0,0,0);	//初始化：开始粒度【时间】
	//计算：粒度总数、粒度时间间隔(单位：s)
	switch(v_cUI_Data.v_iTime)											//查询粒度：分、时、天、周、月
	{
		case 0:															//15分钟
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum*96;					//计算：粒度总数
			v_iTime_Sec=900;											//15分=15*60秒
			if (v_cUI_Data.v_iDay)										//单天有效？？？
				v_iTime_Show = c_iSample_Sum_Min;						//显示粒度数量：每天粒度(mins)
			else
				v_iTime_Show = v_iTime_Sum;								//显示粒度数量：粒度总数
			break;
		case 1:															//时
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum*24;					//计算：粒度总数
			v_iTime_Sec=60*60;											//1小时=60*60秒
			if (v_cUI_Data.v_iDay)										//单天有效？？？
				v_iTime_Show = c_iSample_Sum_Hour;						//显示粒度数量：每天粒度(Hour)
			else
				v_iTime_Show = v_iTime_Sum;								//显示粒度数量：粒度总数
			break;
		case 2:															//天
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum;					//计算：粒度总数
			v_iTime_Sec=24*60*60;										//1天=24*60*60秒
			break;
		case 3:															//周
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum;					//计算：粒度总数
			v_iTime_Sec=7*24*60*60;										//1周=7*24*60*60秒
			v_iTime_Show = v_iTime_Sum;									//显示粒度数量：粒度总数
			break;
		case 4:															//月
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum;					//计算：粒度总数
			v_iTime_Sec=24*60*60;										//1天=24*60*60秒
			v_iTime_Show = v_iTime_Sum;									//显示粒度数量：粒度总数
			break;
	}
	//粒度数据变量申请：依据粒度总数【v_iTime_Sum】
	if (v_dTime_Stamps!=NULL)
		delete[] v_dTime_Stamps;										//清除数据
	v_dTime_Stamps = new double[v_iTime_Sum];							//动态申请
	//粒度数据数组赋值【序号、粒度时间值】
	COleDateTime v_dTime=v_cUI_Data.v_dDate_Start;						//开始日期
	for (int vi=0;vi<v_iTime_Sum;vi++)									//添加时间轴数据 [依据：实际时间节点总数]
	{
		v_dTime_Stamps[vi] = v_dNextDataTime;							//粒度数组赋值：各粒度时间
		v_mTime_Array.insert(pair<double,int>(v_dNextDataTime,vi));		//增加粒度容器：v_mTime_Array [记录粒度与数组位置关系]
		if (v_cUI_Data.v_iTime!=4)
			v_dNextDataTime += v_iTime_Sec;								//v_iTime_Sec：周期 [单位：s]
		else															//月
		{
			v_dTime+=COleDateTimeSpan(1,0,0,0);							//增加1天
			v_dNextDataTime += 24*60*60;								//增加1天[s]
			while (v_dTime.GetDay()!=1)									//每月的1日？？？
			{
				v_dTime+=COleDateTimeSpan(1,0,0,0);						//增加1天
				v_dNextDataTime += 24*60*60;							//增加1天[s]
			}
		}
	}

	//【3】初始化：图形参数
	// Set the full x range to be the duration of the data
	viewer->setFullRange("x", v_dTime_Stamps[0], v_dTime_Stamps[v_iTime_Show - 1]);		//X轴点数：v_iTime_Sum [时间点数量，天数*粒度(1440)]

	// Initialize the view port to show the latest 20% of the time range
	viewer->setViewPortWidth(0.2);
	viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

	// Set the maximum zoom to 10 points
	viewer->setZoomInWidthLimit(10.0 / v_iTime_Show);

	// Initially set the mouse to drag to scroll mode.
	viewer->setMouseUsage(Chart::MouseUsageScroll);

	//设置显示范围：最大范围
	// We do not zoom beyond the zoom width or height limits.
	double newVpWidth = max(getZoomInWidthLimit(), getZoomOutWidthLimit());
	double newVpHeight = max(getZoomInHeightLimit(), getZoomOutHeightLimit());

	// Set the view port position and size so that the point under the mouse remains under
	// the mouse after zooming.
	double deltaX = (getPlotAreaMouseX() - getPlotAreaLeft()) * 
		(getViewPortWidth() - newVpWidth) / getPlotAreaWidth();
	setViewPortLeft(getViewPortLeft() + deltaX);
	setViewPortWidth(newVpWidth);

	double deltaY = (getPlotAreaMouseY() - getPlotAreaTop()) *
		(getViewPortHeight() - newVpHeight) / getPlotAreaHeight();
	setViewPortTop(getViewPortTop() + deltaY);
	setViewPortHeight(newVpHeight);
}

//------------------------------------------------------------------------------------------------------
//	函数功能：在曲线上显示一个粒度的数据信息 [依据鼠标的位置]
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::trackLineLabel(XYChart *c, int mouseX, bool v_bLable)
{
	// Clear the current dynamic layer and get the DrawArea object to draw on it.
	DrawArea *d = c->initDynamicLayer();

	// The plot area object
	PlotArea *plotArea = c->getPlotArea();

	// Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
	double xValue = c->getNearestXValue(mouseX);
	int xCoor = c->getXCoor(xValue);

	// Draw a vertical track line at the x-position
	d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 
		d->dashLineColor(0x000000, 0x0101));

	// Draw a label on the x-axis to show the track line position.
	ostringstream xlabel;
	switch (v_cUI_Data.v_iTime)
	{
		case 0:
			xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "dd_hh:nn")<< " <*/font*>";
			break;
		case 1:
			xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "mm-dd_hh")<< " <*/font*>";
			break;
		case 2:
			xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "yy-mm-dd")<< " <*/font*>";
			break;
		case 3:
			xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "yy-mm-dd")<< " <*/font*>";
			break;
		case 4:
			xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "yyyy-mm")<< " <*/font*>";
			break;
	}
	TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 8);

	// Restrict the x-pixel position of the label to make sure it stays inside the chart image.
	int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
	t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
	t->destroy();	

	// Iterate through all layers to draw the data labels
	for (int i = 0; i < c->getLayerCount(); ++i) {
		Layer *layer = c->getLayerByZ(i);

		// The data array index of the x-value
		int xIndex = layer->getXIndexOf(xValue);

		// Iterate through all the data sets in the layer
		for (int j = 0; j < layer->getDataSetCount(); ++j) 
		{
			DataSet *dataSet = layer->getDataSetByZ(j);
			const char *dataSetName = dataSet->getDataName();

			// Get the color, name and position of the data label
			int color = dataSet->getDataColor();
			int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

			// Draw a track dot with a label next to it for visible data points in the plot area
			if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
				Chart::Transparent) && dataSetName && *dataSetName)
			{
				d->circle(xCoor, yCoor, 4, 4, color, color);

				ostringstream label;
				if(!v_bLable)											//绘图点名称显示 ？？？
					label << "<*font,bgColor=" << hex << color << "*> "	<< c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
				else
					label << "<*font,bgColor=" << hex << color << "*> " << dataSet->getDataName()<< "__" << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";

				t = d->text(label.str().c_str(), "simkai.ttf", 8);

				// Draw the label on the right side of the dot if the mouse is on the left side the
				// chart, and vice versa. This ensures the label will not go outside the chart image.
				if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
					t->draw(xCoor + 5, yCoor, 0xffffff, Chart::Left);
				else
					t->draw(xCoor - 5, yCoor, 0xffffff, Chart::Right);

				t->destroy();
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------
//	函数功能：Get the default background color
//------------------------------------------------------------------------------------------------------
int CMyChartViewer::getDefaultBgColor()
{
	LOGBRUSH LogBrush; 
	HBRUSH hBrush = (HBRUSH)SendMessage(WM_CTLCOLORDLG, (WPARAM)CClientDC(this).m_hDC, 
		(LPARAM)m_hWnd); 
	::GetObject(hBrush, sizeof(LOGBRUSH), &LogBrush); 
	int ret = LogBrush.lbColor;
	
	v_iBgColor=((ret & 0xff) << 16) | (ret & 0xff00) | ((ret & 0xff0000) >> 16);

	return ((ret & 0xff) << 16) | (ret & 0xff00) | ((ret & 0xff0000) >> 16);
}

//------------------------------------------------------------------------------------------------------			
//	函数功能：获取Chart颜色 
//	参    数：v_iNum：依据颜色序号
//	返    回：颜色值
//------------------------------------------------------------------------------------------------------			
int CMyChartViewer::My_GetChart_Color(int v_iNum)
{
	BYTE r3,g3,b3;														//临时变量

	r3 = 0 + v_iNum*111;												//计算：Red
	g3 = 0 + v_iNum*75;													//计算：Green
	b3 = 255 + v_iNum*56;												//计算：Blue

	return r3*256*256+g3*256+b3;										//返回：color
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	类结束：CMyChartViewer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
