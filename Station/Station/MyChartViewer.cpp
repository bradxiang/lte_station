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
//	�ඨ�壺CMyChartViewer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//	���캯��
//-------------------------------------------------------------------------------------------------
CMyChartViewer::CMyChartViewer()
{
	v_dTime_Stamps=NULL;
	v_iTime_Sum=1440;													//Ĭ�ϣ�����������ȫ��������������
}

//-------------------------------------------------------------------------------------------------
//	��������
//-------------------------------------------------------------------------------------------------
CMyChartViewer::~CMyChartViewer()
{
	mChart_Data::iterator v_pIter;
	for(v_pIter=v_mChart_Data.begin();v_pIter!=v_mChart_Data.end();v_pIter++)	//���
		delete[] v_pIter->second.v_dChart_Value;
	v_mChart_Data.clear();												//�������
	v_mTime_Array.clear();												//�������
	if (v_dTime_Stamps!=NULL)
		delete[] v_dTime_Stamps;										//�������
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ͼChart
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::drawChart(CChartViewer *viewer)
{
	CString		v_sTemp,v_Name;											//��ʱ����
	int			startIndex;												//��������ͼ��ʼ��λ�á�ʱ��㡿
	int			endIndex;												//��������ͼ������λ�á�ʱ��㡿
	int			noOfPoints;												//��������ͼ��������ʱ��㡿
	DoubleArray viewPortDataSeriesA;									//��ͼ��������

	//��1����ͼʱ�䴰��ȡ����ͼ��ʱ�����ȷ�Χ����ʼ����(λ��)����������(λ��)��
	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());								//���㣺��ʼʱ��λ�á���ͼ�㡿
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());	//���㣺����ʱ��λ�á���ͼ�㡿

	// Get the array indexes that corresponds to the visible start and end dates
	startIndex = (int)floor(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Show), viewPortStartDate));	//���㣺��ͼ��ʼ��λ�á�ʱ��㡿
	endIndex = (int)ceil(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Show), viewPortEndDate));		//���㣺��ͼ������λ�á�ʱ��㡿
	noOfPoints = endIndex - startIndex + 1;																	//���㣺��ͼ��������ʱ��㡿
	
	// Extract the part of the data array that are visible.
	DoubleArray viewPortTimeStamps = DoubleArray(DoubleArray(v_dTime_Stamps,v_iTime_Show).data + startIndex, noOfPoints);	//��ȡ����ͼX��ʱ���������
 
	//��2��ͼ����������
	int v_iX_A=v_cUI_Data.v_iChart_X-v_cUI_Data.v_iChart_X0-10;			//X����	[10���ұ߿��]
	int v_iY_A=v_cUI_Data.v_iChart_Y-v_cUI_Data.v_iChart_Y0-12;			//Y����	[10���ͱ߿��]

	// Create an XYChart object (v_iX_A x v_iY_A) pixels in size, with light grey (f4f4f4) 
	// background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
    XYChart *c = new XYChart(v_iX_A, v_iY_A, 0xf4f4f4, 0x000000, 1);	//Chart����[v_iX_A,v_iY_A]
    c->setRoundedFrame(v_iBgColor);										//����ɫ
    
    // Set the plotarea at (55, 55) and of size (v_iX_A-20,v_iY_A-20) pixels. Use white (ffffff) 
    // background. Enable both horizontal and vertical grids by setting their colors to 
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	int	v_iY2ax=0;	
//	if (v_cUI_Data.v_iDen || v_cUI_Data.v_iNum)							//��ĸ����ӻ�ͼ������
//		v_iY2ax=60;														//�ұߣ��ճ�60��Y2��
	int	v_iY_A_Time=0;	
    c->setPlotArea(65,80, v_iX_A-90-v_iY2ax, v_iY_A-125-v_iY_A_Time, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);		//��ͼ������
    c->setClipping();

	// Add a legend box at (50, 28) using horizontal layout. Use 10pts Arial Bold as font, with
	// transparent background.
	if (v_mChart_Data.size()<8)											//��������С��8
		c->addLegend(50, 28, false, "simkai.ttf", 10)->setBackground(Chart::Transparent);	//ͼ������


    // Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
    // grey (dddddd) background, black (000000) border, and a glass like raised effect.
	c->addTitle(TCHARtoUTF8(_T(v_cUI_Data.v_sKPI_Name)),"simkai.ttf", 16)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());	//Chart��Ŀ��KPI_Name
            
    // Set the reference font size of the legend box
    c->getLegend()->setFontSize(8);

    //Y������
	if (v_cUI_Data.v_iTime==0)											//����
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Data")), "arialbd.ttf", 10);
	else																//ʱ������
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Data")), "arialbd.ttf", 10);		//Y������

	// set the axis, label and title colors for the primary y axis to green
	// (0x008000) to match the second data set
	c->yAxis2()->setColors(0x008000, 0x008000, 0x008000);

	// Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
    // 15  pixels between minor ticks. This shows more minor grid lines on the chart.
    c->xAxis()->setTickDensity(75, 15);

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

	//��3��X��������ʾ
	// Add a line layer for the lines, using a line width of 2 pixels
	LineLayer *layer = c->addLineLayer();
	layer->setLineWidth(2);
	// In this demo, we do not have too many data points. In real code, the chart may contain a lot
	// of data points when fully zoomed out - much more than the number of horizontal pixels in this
	// plot area. So it is a good idea to use fast line mode.
	layer->setFastLineMode();

	//����װ�أ�X��
    layer->setXData(viewPortTimeStamps);								//��ֵ��X��ʱ�������ݡ���ʾ��ʱ����Ϣ��

	//��4���������� ����������(v_mChart_Data)��ȡ���ݣ���ͼ����
	int						v_iLine_Num=0;								//�����	[��������ɫʱʹ��]
	int						v_iNum=-1;									//��ʼֵ�����߼�����
	int						v_iColor=0x0000FF;							//��ʼֵ����ɫֵ
	CString					v_sInstance_Flag = "";						//���ƣ�Ĭ��Ϊ��
	mChart_Data::iterator	v_pIterator_KPI;							//v_mChart_Data��map����������
	//��ͼ���ݱ���������(����)����������v_mChart_Data��
	v_pIterator_KPI=v_mChart_Data.begin();								//��ֵ��ͷָ��
	while(v_pIterator_KPI != v_mChart_Data.end())						//����(����)��������������
	{
		if (!v_cUI_Data.v_iDay)											//��Ч�����죿����
		{
			//�������
			v_iNum++;													//���߼�����(������ɫ����)
			v_iColor=My_GetChart_Color(v_iNum);							//���㣺������ɫ(�������߼�����)

			//��ȡ����ͼ���ݡ����룺viewPortDataSeriesA��
			viewPortDataSeriesA =DoubleArray(DoubleArray(v_pIterator_KPI->second.v_dChart_Value,v_iTime_Show).data + startIndex, noOfPoints);		//���ݿ�������ͼ���ݷ����ͼ��������viewPortDataSeriesA��

			//��ֵ����ͼ����������Դ��viewPortDataSeriesA��
			if (v_cUI_Data.v_iPoint)									//��Ч����ͼ���ݵ��־����ʾ������+���ơ�
				layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)))->setDataSymbol(Chart::CircleSymbol, 5,0xff0000,0xff0000);	//����һ�����ߣ�����+����
			else														//��Ч����ͼ���ݵ��־��ֻ��ʾ���ݡ�
				layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)));															//����һ�����ߣ�����
			v_pIterator_KPI->second.v_iColor=v_iColor;					//���棺��ͼ��ɫ

			//ȱ���ͼ��(1)�����ߣ�(2)�����ߣ�   [����������Ч�����ߣ���Ч�򲻻���]
			layer->setGapColor(c->dashLineColor(Chart::SameAsMainColor));	//ȱ���ͼ�����ͼ���ɫ
		}
		else															//����
		{
			//������ơ��Ѷ������ݣ���һ�������ϻ��ƣ����ڱȽ�ÿ��仯������
			COleDateTime	v_dDate=v_cUI_Data.v_dDate_Start;			//��ѯ��ʼ����
			for (int vi=0;vi<v_cUI_Data.v_iChart_Day_Sum;vi++)			//��Ч����
			{
				//��������������ԭ��(�����Ҷ��ߣ��������Ʋ�ͬ���������������ÿ�μ�����)
				if (v_cUI_Data.v_iMuti_Line)							//���� �� ���� ������
				{
					if (v_sInstance_Flag!=v_pIterator_KPI->second.v_sName)	//�жϣ����������������Ʋ���ͬ��������ȥ����������6λ(������)��
					{
						v_sInstance_Flag=v_pIterator_KPI->second.v_sName;
						v_iNum++;										//���߼�����
					}
				}
				else
					v_iNum++;											//���߼�����
				v_iColor=My_GetChart_Color(v_iNum);						//���㣺������ɫ(�������߼�����)

				//��ȡ����ͼ���ݡ����룺viewPortDataSeriesA��
				viewPortDataSeriesA =DoubleArray(DoubleArray(v_pIterator_KPI->second.v_dChart_Value,v_iTime_Show).data + v_iTime_Show*vi+startIndex, noOfPoints);		//���ݿ�������ͼ���ݷ����ͼ��������viewPortDataSeriesA��

				//��ֵ����ͼ����������Դ��viewPortDataSeriesA��
				v_sTemp=v_dDate.Format("_%m-%d");						//����
				v_Name = v_pIterator_KPI->second.v_sName + v_sTemp;		//��������+����
				if (v_cUI_Data.v_iPoint)								//��Ч����ͼ���ݵ��־����ʾ������+���ơ�
					layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_Name)))->setDataSymbol(Chart::CircleSymbol, 5,0xff0000,0xff0000);	//����һ�����ߣ�����+����
				else													//��Ч����ͼ���ݵ��־��ֻ��ʾ���ݡ�
					layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_Name)));															//����һ�����ߣ�����
				v_pIterator_KPI->second.v_iColor=v_iColor;				//���棺��ͼ��ɫ

				//ȱ���ͼ��(1)�����ߣ�(2)�����ߣ�   [����������Ч�����ߣ���Ч�򲻻���]
				layer->setGapColor(c->dashLineColor(Chart::SameAsMainColor));	//ȱ���ͼ�����ͼ���ɫ

				v_dDate += COleDateTimeSpan(1,0,0,0);					//����һ��
			}
		}
		v_pIterator_KPI++;												//��������һ
	}

	//��5��ͼ�β������ã�Configure axis scale and labelling
	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	viewer->syncDateAxisWithViewPort("x", c->xAxis());					//���ã�ʱ����

	// If all ticks are yearly aligned, then we use "yyyy" as the label format.
	c->xAxis()->setFormatCondition("align", 360 * 86400);
	c->xAxis()->setLabelFormat("{value|yyyy}");

	//����Y��̶�
// 	if (v_cUI_Data.v_iY_scale)
// 	{
// 		CString v_sMax,v_sMin;
// 		c->yAxis()->setDateScale(v_cUI_Data.v_iMin,v_cUI_Data.v_iMax,0,0);		//���ã�Y��̶�
// 	}
// 	else
// 		c->yAxis()->setDateScale();										//���ã�Y���Զ��̶�
// 	c->yAxis2()->setDateScale();										//���ã�Y2���Զ��̶�

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
//	�������ܣ���ͼChart  [Bar]
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::drawChart_Bar(CChartViewer *viewer)
{
	int	startIndex;														//���룺ʵ�����ݵ���
	int endIndex;														//���룺ʵ�����ݵ���
	int noOfPoints;														//

	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());

	// Get the array indexes that corresponds to the visible start and end dates
	startIndex = (int)floor(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Sum), viewPortStartDate));	//���룺ʵ�����ݵ���
	endIndex = (int)ceil(Chart::bSearch(DoubleArray(v_dTime_Stamps,v_iTime_Sum), viewPortEndDate));			//���룺ʵ�����ݵ���
	noOfPoints = endIndex - startIndex + 1;

	// Extract the part of the data array that are visible.
	DoubleArray viewPortTimeStamps = DoubleArray(DoubleArray(v_dTime_Stamps,v_iTime_Sum).data + startIndex, noOfPoints);

	int v_iX_A=v_cUI_Data.v_iChart_X-v_cUI_Data.v_iChart_X0-10;			//X����	[10���ұ߿��]
	int v_iY_A=v_cUI_Data.v_iChart_Y-v_cUI_Data.v_iChart_Y0-12;			//Y����	[10���ͱ߿��]

	// Create an XYChart object (v_iX_A x v_iY_A) pixels in size, with light grey (f4f4f4) 
	// background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
	XYChart *c = new XYChart(v_iX_A, v_iY_A, 0xf4f4f4, 0x000000, 1);	//Chart����[v_iX_A,v_iY_A]
	c->setRoundedFrame(v_iBgColor);										//����ɫ

	// Set the plotarea at (55, 55) and of size (v_iX_A-20,v_iY_A-20) pixels. Use white (ffffff) 
	// background. Enable both horizontal and vertical grids by setting their colors to 
	// grey (cccccc). Set clipping mode to clip the data lines to the plot area.
	int	v_iY2ax=0;	
	//	if (v_cUI_Data.v_iDen || v_cUI_Data.v_iNum)							//��ĸ����ӻ�ͼ������
	//		v_iY2ax=60;														//�ұߣ��ճ�60��Y2��
	int	v_iY_A_Time=0;	
	c->setPlotArea(65,80, v_iX_A-90-v_iY2ax, v_iY_A-125-v_iY_A_Time, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);		//��ͼ������
	c->setClipping();

	// Add a legend box at (50, 28) using horizontal layout. Use 10pts Arial Bold as font, with
	// transparent background.
	if (v_mChart_Data.size()<8)											//��������С��8
		c->addLegend(50, 28, false, "simkai.ttf", 10)->setBackground(Chart::Transparent);	//ͼ������


	// Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
	// grey (dddddd) background, black (000000) border, and a glass like raised effect.
	c->addTitle(TCHARtoUTF8(_T(v_cUI_Data.v_sKPI_Name)),"simkai.ttf", 16)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());	//Chart��Ŀ��KPI_Name

	// Set the reference font size of the legend box
	c->getLegend()->setFontSize(8);

	//Y������
	if (v_cUI_Data.v_iTime==0)											//AP����
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Amounts")), "arialbd.ttf", 10);
	else																//ʱ������
		c->yAxis()->setTitle(TCHARtoUTF8(_T("Capacity (GBytes)")), "arialbd.ttf", 10);		//Y������

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
	
	if (v_cUI_Data.v_iBar_3d)											//3D��Ч������
		layer->set3D(10);												//���ã�3D����

	//����װ�أ�X��
	layer->setXData(viewPortTimeStamps);								//X�᣺ʱ����

	int	v_iLine_Num=0;													//�����	[��������ɫʱʹ��]
	// ����װ�أ�����
	int		v_iNum=-1;													//��ɫ��ţ���ʼֵ
	mChart_Data::iterator v_pIterator_KPI;								//KPI_Instance��map����������
	v_pIterator_KPI=v_mChart_Data.begin();								//��ͷ��ʼ
	int	v_iColor=0x0000FF;												//��ɫֵ
	CString	v_sInstance_Flag="";										//���ƣ�Ĭ��Ϊ��
	while(v_pIterator_KPI != v_mChart_Data.end())						//����������
	{
		if (v_cUI_Data.v_iDay && v_cUI_Data.v_iMuti_Line)
		{
			CString	v_sTemp=v_pIterator_KPI->second.v_sName;
			if (v_sInstance_Flag!=v_sTemp.Left(v_sTemp.GetLength()-6))
			{
				v_sInstance_Flag=v_sTemp.Left(v_sTemp.GetLength()-6);
				v_iNum++;												//��ɫ���
			}
		}
		else
			v_iNum++;													//��ɫ���

		//KPI_Value
		DoubleArray viewPortDataSeriesA,viewPortDataSeriesB;			//��ͼ����
		v_iColor=My_GetChart_Color(v_iNum);								//������ɫ
		viewPortDataSeriesA =DoubleArray(DoubleArray(v_pIterator_KPI->second.v_dChart_Value,v_iTime_Sum).data + startIndex, noOfPoints);		//���ݿ���

		if (v_cUI_Data.v_iPoint)										//��Ч����ͼ���ݵ��־
			layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)))->setDataSymbol(Chart::CircleSymbol, 5,0xff0000,0xff0000);		//����һ�����ߣ�����+����
		else
			layer->addDataSet(viewPortDataSeriesA, v_iColor , TCHARtoUTF8(_T(v_pIterator_KPI->second.v_sName)));																//����һ�����ߣ�����+����	

		switch(v_cUI_Data.v_iBar_Type)									//Bar����
		{
			case 0:														//Bar: ����
				layer->setBarShape(Chart::SquareShape);					//Ĭ��
				break;
			case 1:														//Bar: Բ��
				layer->setBarShape(Chart::CircleShape);
				break;
			case 2:														//Bar: ����
				layer->setBarShape(Chart::StarShape(5));
				break;
			case 3:														//Bar: ����
				layer->setBarShape(Chart::PolygonShape(6));
				break;
		}

		v_pIterator_KPI->second.v_iColor=v_iColor;						//���棺��ͼ��ɫ
		//ȱ���ͼ��(1)�����ߣ�(2)�����ߣ�   [����������Ч�����ߣ���Ч�򲻻���]
		//		layer->setGapColor(c->dashLineColor(Chart::SameAsMainColor));	//ȱ���ͼ�����ͼ���ɫ

		v_pIterator_KPI++;												//��������һ
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// Configure axis scale and labelling
	///////////////////////////////////////////////////////////////////////////////////////
	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	viewer->syncDateAxisWithViewPort("x", c->xAxis());					//���ã�ʱ����

	// If all ticks are yearly aligned, then we use "yyyy" as the label format.
	c->xAxis()->setFormatCondition("align", 360 * 86400);
	c->xAxis()->setLabelFormat("{value|yyyy}");

	//����Y��̶�
// 	if (v_cUI_Data.v_iY_scale)
// 	{
// 		CString v_sMax,v_sMin;
// 		c->yAxis()->setDateScale(v_cUI_Data.v_iMin,v_cUI_Data.v_iMax,0,0);		//���ã�Y��̶�
// 	}
// 	else
// 		c->yAxis()->setDateScale();										//���ã�Y���Զ��̶�
// 	c->yAxis2()->setDateScale();										//���ã�Y2���Զ��̶�

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
//	�������ܣ�Initialize the CChartViewer
//	��    ������1�����ݳ�ʼ������2��ʱ�������ݳ�ʼ������3����ʼ����ͼ�β�����
//------------------------------------------------------------------------------------------------------
void CMyChartViewer::initChartViewer(CChartViewer *viewer)
{
	int			v_iTime_Sec;											//��ʱ����
	double		v_dNextDataTime;										//ʱ����

	//��1�����ݳ�ʼ��
	mChart_Data::iterator v_pIter;
	for(v_pIter=v_mChart_Data.begin();v_pIter!=v_mChart_Data.end();v_pIter++)	//���
		delete[] v_pIter->second.v_dChart_Value;
	v_mChart_Data.clear();												//������λ����������(���ơ����ݡ���ɫ)��
	v_mTime_Array.clear();												//������λ��ʱ����������š�

	//��2��ʱ�������ݳ�ʼ�������㣺����������X��ʱ�����顿
	v_dNextDataTime = Chart::chartTime(v_cUI_Data.v_dDate_Start.GetYear(),v_cUI_Data.v_dDate_Start.GetMonth(),v_cUI_Data.v_dDate_Start.GetDay(),0,0,0);	//��ʼ������ʼ���ȡ�ʱ�䡿
	//���㣺��������������ʱ����(��λ��s)
	switch(v_cUI_Data.v_iTime)											//��ѯ���ȣ��֡�ʱ���졢�ܡ���
	{
		case 0:															//15����
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum*96;					//���㣺��������
			v_iTime_Sec=900;											//15��=15*60��
			if (v_cUI_Data.v_iDay)										//������Ч������
				v_iTime_Show = c_iSample_Sum_Min;						//��ʾ����������ÿ������(mins)
			else
				v_iTime_Show = v_iTime_Sum;								//��ʾ������������������
			break;
		case 1:															//ʱ
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum*24;					//���㣺��������
			v_iTime_Sec=60*60;											//1Сʱ=60*60��
			if (v_cUI_Data.v_iDay)										//������Ч������
				v_iTime_Show = c_iSample_Sum_Hour;						//��ʾ����������ÿ������(Hour)
			else
				v_iTime_Show = v_iTime_Sum;								//��ʾ������������������
			break;
		case 2:															//��
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum;					//���㣺��������
			v_iTime_Sec=24*60*60;										//1��=24*60*60��
			break;
		case 3:															//��
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum;					//���㣺��������
			v_iTime_Sec=7*24*60*60;										//1��=7*24*60*60��
			v_iTime_Show = v_iTime_Sum;									//��ʾ������������������
			break;
		case 4:															//��
			v_iTime_Sum=v_cUI_Data.v_iChart_Day_Sum;					//���㣺��������
			v_iTime_Sec=24*60*60;										//1��=24*60*60��
			v_iTime_Show = v_iTime_Sum;									//��ʾ������������������
			break;
	}
	//�������ݱ������룺��������������v_iTime_Sum��
	if (v_dTime_Stamps!=NULL)
		delete[] v_dTime_Stamps;										//�������
	v_dTime_Stamps = new double[v_iTime_Sum];							//��̬����
	//�����������鸳ֵ����š�����ʱ��ֵ��
	COleDateTime v_dTime=v_cUI_Data.v_dDate_Start;						//��ʼ����
	for (int vi=0;vi<v_iTime_Sum;vi++)									//���ʱ�������� [���ݣ�ʵ��ʱ��ڵ�����]
	{
		v_dTime_Stamps[vi] = v_dNextDataTime;							//�������鸳ֵ��������ʱ��
		v_mTime_Array.insert(pair<double,int>(v_dNextDataTime,vi));		//��������������v_mTime_Array [��¼����������λ�ù�ϵ]
		if (v_cUI_Data.v_iTime!=4)
			v_dNextDataTime += v_iTime_Sec;								//v_iTime_Sec������ [��λ��s]
		else															//��
		{
			v_dTime+=COleDateTimeSpan(1,0,0,0);							//����1��
			v_dNextDataTime += 24*60*60;								//����1��[s]
			while (v_dTime.GetDay()!=1)									//ÿ�µ�1�գ�����
			{
				v_dTime+=COleDateTimeSpan(1,0,0,0);						//����1��
				v_dNextDataTime += 24*60*60;							//����1��[s]
			}
		}
	}

	//��3����ʼ����ͼ�β���
	// Set the full x range to be the duration of the data
	viewer->setFullRange("x", v_dTime_Stamps[0], v_dTime_Stamps[v_iTime_Show - 1]);		//X�������v_iTime_Sum [ʱ�������������*����(1440)]

	// Initialize the view port to show the latest 20% of the time range
	viewer->setViewPortWidth(0.2);
	viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

	// Set the maximum zoom to 10 points
	viewer->setZoomInWidthLimit(10.0 / v_iTime_Show);

	// Initially set the mouse to drag to scroll mode.
	viewer->setMouseUsage(Chart::MouseUsageScroll);

	//������ʾ��Χ�����Χ
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
//	�������ܣ�����������ʾһ�����ȵ�������Ϣ [��������λ��]
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
				if(!v_bLable)											//��ͼ��������ʾ ������
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
//	�������ܣ�Get the default background color
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
//	�������ܣ���ȡChart��ɫ 
//	��    ����v_iNum��������ɫ���
//	��    �أ���ɫֵ
//------------------------------------------------------------------------------------------------------			
int CMyChartViewer::My_GetChart_Color(int v_iNum)
{
	BYTE r3,g3,b3;														//��ʱ����

	r3 = 0 + v_iNum*111;												//���㣺Red
	g3 = 0 + v_iNum*75;													//���㣺Green
	b3 = 255 + v_iNum*56;												//���㣺Blue

	return r3*256*256+g3*256+b3;										//���أ�color
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	�������CMyChartViewer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
