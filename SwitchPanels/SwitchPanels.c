//==============================================================================
//
// Title:		SwitchPanels
// Purpose:		A short description of the application.
//
// Created on:	11/5/2020 at 4:51:51 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include<analysis.h> // pt min , max
#include <formatio.h>
#include "toolbox.h"
#include "SwitchPanels.h"
#include "Acquisition.h"


//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================
// Types

//==============================================================================
// Static global variables



//==============================================================================
// Static functions

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements

double *waveData = 0; // stochez valorile esantioanelor
double *filter=0;
double *anvelopa=0;
int wavePanel = 0;
//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (wavePanel = LoadPanel (0, "SwitchPanels.uir", WAVE_PANEL));
	errChk (acqPanel = LoadPanel (0, "SwitchPanels.uir",FREC_PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (wavePanel));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (wavePanel > 0)
		DiscardPanel (wavePanel);
		
	return 0;
}




//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK OnWavePanelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == wavePanel)
			{
				SetCtrlVal(acqPanel, FREC_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(acqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(wavePanel, FREC_PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int i=1;
	int zero=0;
	int intervals=10;
	double axis[100];
	int his[100]; //stochez frecv esantioane
	double variance;
	double mean;
	ssize_t minIndex;
	double min;
	ssize_t maxIndex;
	double max;
	double val_mediana;
	
	double *x;
	double skew=0.0;
 	double kurtosis=0.0;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			//Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			
			waveData = (double *) calloc(npoints, sizeof(double));
			
			filter = (double *) calloc(npoints, sizeof(double));
			
			x= (double *) calloc(256, sizeof(double));
			
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			
			
		
			PlotY(panel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			MaxMin1D (waveData, npoints, &max, &maxIndex, &min, &minIndex);
			Histogram(waveData,npoints,min-1,max+1,his,axis,intervals);
			Mean (waveData, npoints, &mean);
			Median(waveData, npoints, &val_mediana);
			Variance (waveData, npoints, &mean, &variance);
						
// de cate ori trece prin zero
			
			while(i<npoints){
				if((waveData[i]==0)||(waveData[i-1]>0&&waveData[i]<0) ||(waveData[i]>0 && waveData[i-1]<0) )
				{
					++zero;
				}
				++i;
			}
			
			
			
			
			DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
			PlotXY (wavePanel, WAVE_PANEL_IDC_GRAPH_HISTOGRAM, axis, his, intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
			SetCtrlVal (wavePanel, WAVE_PANEL_MIN, min);
			SetCtrlVal (wavePanel, WAVE_PANEL_MAX, max);
			SetCtrlVal (wavePanel, WAVE_PANEL_MEAN,mean);
			SetCtrlVal (wavePanel, WAVE_PANEL_IDC_MEDIAN, val_mediana);
			SetCtrlVal (wavePanel, WAVE_PANEL_IDC_MAX_INDEX,maxIndex);
			SetCtrlVal (wavePanel, WAVE_PANEL_IDC_MIN_INDEX,minIndex);
			SetCtrlVal (wavePanel, WAVE_PANEL_VARIANCE, variance);
			SetCtrlVal (wavePanel, WAVE_PANEL_NZERO,zero);
			
			for(int i=0;i<255;++i)
			{
				x[i]=waveData[i];
				
			}
			Moment (x, 256, 3, &skew);
			Moment (x, 256, 4, &kurtosis);
			
			
			SetCtrlVal (wavePanel, WAVE_PANEL_IDC_ASIMETRIA, skew);
			SetCtrlVal (wavePanel, WAVE_PANEL_IDC_APLATIZAREA, kurtosis);
			break;
	}
	
	return 0;
}
//filtre
//mereu ma mut cu elemnt in fata , elimin el precedent 
double * filtrare_mediere(int n){
	double suma=0;
	double *filter=(double*)malloc(npoints*sizeof(double));
	for(int i=0;i<n-1;i++)
	{
		suma+=waveData[i];
	}
	// s1=s0-U0+U8
	// elimin el care nu se vede in fereastra adun el nou-> j+n
	for(int j=0;j<npoints-n;++j)
	{
		suma=suma-waveData[j]+waveData[j+n];
		filter[j]=suma/n;
	}
	for(int k=npoints-n;k<npoints;++k)
	{
		filter[k]=suma/n;
	}
	return filter;
}

double *filtrare_ordin1(double alpha)
{
	double *filter=(double*)malloc(npoints*sizeof(double));
	for(int i=1;i<npoints;++i)
	{
		filter[i]=(1-alpha)*filter[i-1]+alpha*waveData[i];
	}
	return filter;
}

int CVICALLBACK selectFilter (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int valFiltru,nrElemente;
	double alpha;
	switch (event)
	{
			
		case EVENT_COMMIT:
			GetCtrlVal(wavePanel, WAVE_PANEL_FILTER, &valFiltru);
			if(valFiltru==0){
				SetCtrlAttribute (wavePanel, WAVE_PANEL_NELEMENTE_IDC, ATTR_DIMMED, !valFiltru);
				// activez contrl alpha
				SetCtrlAttribute (wavePanel, WAVE_PANEL_ALPHA_IDC, ATTR_DIMMED, valFiltru);
				//extrag val de pe interfata pt alpha
				GetCtrlVal(wavePanel, WAVE_PANEL_ALPHA_IDC, &alpha);
				filter=filtrare_ordin1(alpha);
			}else{
				SetCtrlAttribute(wavePanel,WAVE_PANEL_NELEMENTE_IDC,ATTR_DIMMED, !valFiltru);
				SetCtrlAttribute (wavePanel, WAVE_PANEL_ALPHA_IDC, ATTR_DIMMED, valFiltru);
				//extrag val de pe interfata pt alpha
				GetCtrlVal(wavePanel, WAVE_PANEL_NELEMENTE_IDC, &nrElemente);
				filter=filtrare_mediere(nrElemente);
			}
			break;
	}
	return 0;
}



int CVICALLBACK APLICA_CB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			selectFilter(panel,WAVE_PANEL_FILTER , EVENT_COMMIT, 0, 0, 0);
			DeleteGraphPlot(panel, WAVE_PANEL_IDC_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
			PlotY (panel, WAVE_PANEL_IDC_GRAPH_FILTERED, filter, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);
			break;
	}
	return 0;
}


void saveImage(void){
	int bitmapID;
	char fileName[256]={0};
	char fileName1[256]={0};
	sprintf(fileName, "D:\proiect APD\\grafic_nefiltrat.jpg"); //generare nume fisier
	GetCtrlDisplayBitmap (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, 1, &bitmapID);
	SaveBitmapToJPEGFile(bitmapID, fileName, JPEG_PROGRESSIVE, 100);
	sprintf(fileName1, "D:\proiect APD\\grafic_filtrat.jpg"); //generare nume fisier
	GetCtrlDisplayBitmap (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTERED, 1, &bitmapID);
	SaveBitmapToJPEGFile(bitmapID, fileName1, JPEG_PROGRESSIVE, 100);
}

int CVICALLBACK NEXT_CB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	double *temp;
	double *temp1;
	temp=(double*)calloc(npoints/6,sizeof(double));
	temp1=(double*)calloc(npoints/6,sizeof(double));temp=(double*)calloc(npoints/6,sizeof(double));
		
	int start_val=0,stop_val=0;
	switch (event)
	{  
		case EVENT_COMMIT:
		GetCtrlVal (wavePanel, WAVE_PANEL_START_IDC, &start_val);
		GetCtrlVal (wavePanel, WAVE_PANEL_STOP_IDC, &stop_val);	
			if(start_val>=0 && stop_val<6){
				SetCtrlVal (wavePanel, WAVE_PANEL_START_IDC, ++start_val);
				SetCtrlVal (wavePanel, WAVE_PANEL_STOP_IDC, ++stop_val);
				for(int i=0;i<npoints/6;++i)
								{
									temp[i]=waveData[start_val*npoints/6+i];
								}
								
								for(int i=0;i<npoints/6;++i)
								{
									temp1[i]=filter[start_val*npoints/6+i];
								}
								DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
								PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, temp, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								
								DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
								PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTERED, temp1, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_MAGENTA);
			}
				saveImage();			
				free (temp);
				free( temp1);
			break;
	
	}
	return 0;
}

int CVICALLBACK PREV_CB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	double *temp;
	double *temp1;
	temp=(double*)calloc(npoints/6,sizeof(double));
	temp1=(double*)calloc(npoints/6,sizeof(double));temp=(double*)calloc(npoints/6,sizeof(double));
		
	int start_val=0,stop_val=0;
	
	
	switch (event)
	{  
		case EVENT_COMMIT:
		GetCtrlVal (wavePanel, WAVE_PANEL_START_IDC, &start_val);
		GetCtrlVal (wavePanel, WAVE_PANEL_STOP_IDC, &stop_val);	
			if(start_val>0){
				SetCtrlVal (wavePanel, WAVE_PANEL_START_IDC,--start_val);
				SetCtrlVal (wavePanel, WAVE_PANEL_STOP_IDC, --stop_val);
				for(int i=0;i<npoints/6;++i)
								{
									temp[i]=waveData[start_val*npoints/6+i];
								}
								
								for(int i=0;i<npoints/6;++i)
								{
									temp1[i]=filter[start_val*npoints/6+i];
									
								}
								
								
								DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
								PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, temp, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								
								DeleteGraphPlot (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTERED, -1, VAL_IMMEDIATE_DRAW);
								PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_FILTERED, temp1, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_MAGENTA);
			}
			saveImage();	
			free(temp);
			free(temp1);
				
			break;
	}
	return 0;
}





int CVICALLBACK ANVELOPA_CB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//LaunchExecutable("python anvelopa.py");
			
			anvelopa = (double *) calloc(npoints, sizeof(double));   
			
			
			FileToArray("anvelopa.txt", anvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY (wavePanel, WAVE_PANEL_IDC_GRAPH_RAW_DATA, anvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			break;
	}
	return 0;
}



