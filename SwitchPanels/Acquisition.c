//==============================================================================
//
// Title:		Acquisition.c
// Purpose:		A short description of the implementation.
//
// Created on:	11/5/2020 at 5:01:06 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <advanlys.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Acquisition.h"
#include "SwitchPanels.h"

 

//==============================================================================
// Constants
#define	FIR_KSR_HPF 0
#define	CHEBYSHEV_INVERS 1
#define HAMMING 0
#define BLACKMAN_HARRIS 1
//==============================================================================
// Types
  int N;//numarul de puncte pe care il folosim pentru a reprezenta spectrul semnalului
  int tip_filtru;
  int tip_fereastra;
  int imgHandle;
//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double dt; //pas in domeniul timp
double* autoSpectrum;
double* convertedSpectrum;
double df;
WindowConst winConst;
//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
double powerPeak = 0.0;
	//valoarea maxima din spectru de putere (din autoSpectrum)
double frecPeak = 0.0;
//voltage signal - descriere a semnalului achizitionat
char unit[128] = "V";


//==============================================================================
// Global functions

int CVICALLBACK OnFrecPanelCB (int panel, int event, void *callbackData,
									  int eventData1, int eventData2)
{
	
	
	switch (event)
	{
			
	   
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK FrequencyCB (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	GetCtrlVal (acqPanel, FREC_PANEL_IDC_NUMAR_PUNCTE, &N);
	SetCtrlVal (acqPanel, FREC_PANEL_IDC_N_POINTS,npoints);
	SetCtrlVal (acqPanel, FREC_PANEL_IDC_SAMPLERATE, sampleRate);
	double *raw;
	int secunda;
	//double*buffer=waveData + (unsigned int)(sampleRate);
	
	// dt trebuie sa fie 1.0/dimensiune buffer
	dt = 1/N;
	 
	convertedSpectrum = (double *) calloc(N/2 , sizeof(double)); // vectorul utilizat pt reprezentarea spectrului
	autoSpectrum = (double *) calloc(N/2, sizeof(double)); ////spectrul de putere cu nr de val = 1/2 din dim bufferului de int(inputArray); 
	switch (event)
	{
		
		case EVENT_COMMIT:
		
			
			
			
		
		
		//? în general o fereastr? cu scopul de a “aplatiza” forma semnalului la capetele intervalului de e?antioane analizat.
	
		DeleteGraphPlot (acqPanel, FREC_PANEL_IDC_GRAPH_FREC_RAW,-1, VAL_IMMEDIATE_DRAW);
		PlotY (acqPanel,FREC_PANEL_IDC_GRAPH_FREC_RAW, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE,VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		
		
		
		ScaledWindow(waveData, N, RECTANGLE_, &winConst);
		AutoPowerSpectrum (waveData,N, dt, autoSpectrum, &df);
		//calculeaza puterea si frecv corespunzatoare varfului din spectrul semnalului  
		PowerFrequencyEstimate(autoSpectrum, N/2, -1.0, winConst, df,7, &frecPeak,&powerPeak);
		 ////spectrul de putere cu nr de val = 1/2 din dim bufferului de int(inputArray); 
		SetCtrlVal (acqPanel,FREC_PANEL_FREC_PEAK_IDC, frecPeak);
		SetCtrlVal(acqPanel, FREC_PANEL_POWER_PEAK_IDC,powerPeak);
		
		SpectrumUnitConversion(autoSpectrum, N/2 , 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum,unit);
		
		DeleteGraphPlot (acqPanel, FREC_PANEL_IDC_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);
		PlotWaveform (acqPanel, FREC_PANEL_IDC_GRAPH_SPECTRU, convertedSpectrum, N/2, VAL_DOUBLE, 1.0, 0.0, 0.0, 1, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_GREEN);
		
		
		
			break;
	}
	return 0;
}

int CVICALLBACK FREC_NEXT_CB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int start_val=0,stop_val=0;
	switch (event)
	{
		case EVENT_COMMIT:
		{
			GetCtrlVal (acqPanel, FREC_PANEL_START_FREC_IDC, &start_val);
			GetCtrlVal (acqPanel, FREC_PANEL_STOP_FREC_IDC, &stop_val);
			if(start_val>=0 && stop_val<6)	
			{
					SetCtrlVal (acqPanel,FREC_PANEL_START_FREC_IDC, ++start_val);
					SetCtrlVal (acqPanel,FREC_PANEL_STOP_FREC_IDC , ++stop_val);
			}
			
		}	
		break;
	}
	return 0;
}

int CVICALLBACK FREC_PREV_CB (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int start_val=0,stop_val=0;
	switch (event)
	{
		case EVENT_COMMIT:
		{
			GetCtrlVal (acqPanel, FREC_PANEL_START_FREC_IDC, &start_val);
			GetCtrlVal (acqPanel, FREC_PANEL_STOP_FREC_IDC, &stop_val);
			if(start_val>0 )	
			{
					SetCtrlVal (acqPanel, FREC_PANEL_START_FREC_IDC,--start_val);
					SetCtrlVal (acqPanel, FREC_PANEL_STOP_FREC_IDC, --stop_val);
			}
			
		}	
			break;
	}
	return 0;
}



int CVICALLBACK Filter_CB (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{	
	
	int secunda;
	double *raw;
	
	double *autoSpectrumFereastra;
	double *convertedSpectrumFereastra;
	double coef[141];
	double fcut,fpass;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
		//1.FERESTRUIREA	
		GetCtrlVal (acqPanel, FREC_PANEL_IDC_NUMAR_PUNCTE, &N);// nr de puncte pt ferestruire
		GetCtrlVal (acqPanel, FREC_PANEL_FCUT_IDC, &fcut);
		GetCtrlVal (acqPanel, FREC_PANEL_FPASS_IDC, &fpass);
		
		SetCtrlVal (acqPanel, FREC_PANEL_IDC_N_POINTS,npoints);
		SetCtrlVal (acqPanel, FREC_PANEL_IDC_SAMPLERATE, sampleRate);
		autoSpectrumFereastra=(double*)calloc(N/2,sizeof(double));
		convertedSpectrumFereastra=(double*)calloc(N/2,sizeof(double));
		
		double window[N];
		double final[N];
	
			
			GetCtrlVal (acqPanel, FREC_PANEL_START_FREC_IDC, &secunda); 
			
			raw=(double*)calloc(N,sizeof(double));	
			
			
			for(int i=0;i<N;i++)
			{
				if(secunda*npoints/6+i<npoints)
				{
					raw[i]=waveData[secunda*npoints/6+i];
				}
				else
				{
					raw[i]=0;
				}
			}
			
			GetCtrlVal(acqPanel,FREC_PANEL_TIP_FEREASTRA,&tip_fereastra);
			LinEv1D(raw,N,0,1,window);
			switch(tip_fereastra)
			{
				case HAMMING:
				
					HamWin (window,N);
					
				break;
				case BLACKMAN_HARRIS:
					
					BlkHarrisWin(window,N);
				break;			
					
		}
		//Afisare forma fereastra
		DeleteGraphPlot (acqPanel, FREC_PANEL_IDC_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
		PlotY (acqPanel,FREC_PANEL_IDC_GRAPH_WINDOW, window, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		//Ferestruire pe semnal si afisare
		Mul1D(raw,window,N,final); //pe iesirea la asta aplic filtrul
		DeleteGraphPlot (acqPanel, FREC_PANEL_GRAPH_SIG_RAW_WINDOW, -1, VAL_IMMEDIATE_DRAW);
		PlotY (acqPanel, FREC_PANEL_GRAPH_SIG_RAW_WINDOW, final, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		
		//afisare spectru dupa ferestruire semnal 
		
		dt=1/N;
		AutoPowerSpectrum (final,N, dt,autoSpectrumFereastra, &df);
		//calculeaza puterea si frecv corespunzatoare varfului din spectrul semnalului  
		PowerFrequencyEstimate(autoSpectrumFereastra, N/2, -1.0, winConst, df,7, &frecPeak,&powerPeak);
		 ////spectrul de putere cu nr de val = 1/2 din dim bufferului de int(inputArray); 
		SetCtrlVal (acqPanel,FREC_PANEL_FREC_PEAK_IDC, frecPeak);
		SetCtrlVal(acqPanel, FREC_PANEL_POWER_PEAK_IDC,powerPeak);
		
		SpectrumUnitConversion(autoSpectrumFereastra, N/2 , 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst,  convertedSpectrumFereastra,unit);
		DeleteGraphPlot (acqPanel, FREC_PANEL_IDC_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);
		PlotWaveform (acqPanel, FREC_PANEL_IDC_GRAPH_SPECTRU, convertedSpectrumFereastra, N/2, VAL_DOUBLE, 1.0, 0.0, 0.0, 1, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_GREEN);
		
		//extragem de pe interfata ->tipul de filtru dorit
		GetCtrlVal (acqPanel, FREC_PANEL_FILTER_TYPE, &tip_filtru);
		
		//filtrare
		double *semnal_filtrat;
		semnal_filtrat = (double *) calloc(npoints, sizeof(double));
			
		
		switch (tip_filtru) {
				//trebuie început cu proiectarea acestuia ->determinam ordinul coeficientilor filtrului
		case FIR_KSR_HPF:
				Ksr_HPF (sampleRate, fcut, 141, coef,4.5); // imi genereaza doar coeficientii -> nu si filtrarea 
				Convolve(final,N,coef,141,semnal_filtrat);
			
				//filtrarea se va realiza în urma operatiei de convolutie dintre semnalul dat si vectorul aferent filtrului
				//Convolutia este o operatie formal matematica care combina 2 semnale pentru a forma un alt semnal
				
			break; 
		case CHEBYSHEV_INVERS:
				InvCh_HPF (final,N,sampleRate,fcut,80,6,semnal_filtrat);
			break;
		
		}
		double *autoSpectrumFiltru = (double *) calloc(N/2,sizeof(double));
		double *convertedSpectrumFiltru = (double *) calloc(N/2, sizeof(double));
		
		//AFISARE filtrare pentru SEMNAL pe fereastra 
		Mul1D(semnal_filtrat,window,N,final);
		DeleteGraphPlot (acqPanel, FREC_PANEL_IDC_GRAPH_FILTRED, -1, VAL_IMMEDIATE_DRAW);
		PlotY (acqPanel,FREC_PANEL_IDC_GRAPH_FILTRED, final, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		
		//AFIsare spectru pentru SEMNAL FILTRAT
		AutoPowerSpectrum (final,N, dt, autoSpectrumFiltru, &df);
		SpectrumUnitConversion(autoSpectrumFiltru, N/2 , 0, SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrumFiltru,unit);
		DeleteGraphPlot (acqPanel, FREC_PANEL_IDC_GRAPH_FIL_SPECTRU, -1, VAL_IMMEDIATE_DRAW);
		PlotWaveform (acqPanel, FREC_PANEL_IDC_GRAPH_FIL_SPECTRU, convertedSpectrumFiltru, N/2, VAL_DOUBLE, 1.0, 0.0, 0.0, 1, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_GREEN);
		
		
	}
	return 0;
}

int CVICALLBACK CB_SALVARE_IMAGINI (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{	
	char fileName[256]={0};
	char nume_fereastra[50]={0}, nume_filtru[50]={0};
	int secunda;
	switch (event)
	{
		case EVENT_COMMIT:
		GetCtrlVal (acqPanel, FREC_PANEL_FILTER_TYPE, &tip_filtru);
		GetCtrlVal(acqPanel,FREC_PANEL_TIP_FEREASTRA,&tip_fereastra);
		GetCtrlVal(acqPanel,FREC_PANEL_START_FREC_IDC,&secunda);
		switch (tip_fereastra){
			case HAMMING:
			
				strcpy(nume_fereastra,"HAMMING");
				
				break;
			case BLACKMAN_HARRIS:
				strcpy(nume_fereastra,"Blackman-Harris");
				
				break;
		}
		switch (tip_filtru){
			case CHEBYSHEV_INVERS:
			
				strcpy(nume_filtru,"CHEBYSHEV_INVERS de ordin 6");
				
				break;
			case FIR_KSR_HPF:
				strcpy(nume_filtru,"FIR_KSR_HPF");
				
				break;
		}
		
		
		
				sprintf(fileName, "Semnal_nefiltrat_%s_secunda_%d-%d.jpeg", nume_fereastra,secunda,secunda+1);
				GetCtrlDisplayBitmap(acqPanel,FREC_PANEL_IDC_GRAPH_FREC_RAW, 1, &imgHandle);
				SaveBitmapToJPEGFile(imgHandle, fileName, JPEG_DCTFAST, 100);
				DiscardBitmap(imgHandle);
				//salvare semnal filtrat
				sprintf(fileName, "Semnal_filtrat_%s_%s_secunda_%d-%d.jpeg",nume_filtru, nume_fereastra,secunda, secunda+1);
				GetCtrlDisplayBitmap(acqPanel,FREC_PANEL_IDC_GRAPH_FILTRED, 1, &imgHandle);
				SaveBitmapToJPEGFile(imgHandle, fileName, JPEG_DCTFAST, 100);
				DiscardBitmap(imgHandle);
		
		break;
	}
	return 0;
}
