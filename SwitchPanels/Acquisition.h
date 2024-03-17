//==============================================================================
//
// Title:		Acquisition.h
// Purpose:		A short description of the interface.
//
// Created on:	11/5/2020 at 5:01:06 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Acquisition_H__
#define __Acquisition_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files
#include <userint.h>
#include "cvidef.h"

double *waveData;
//int npoints;
//double sampleRate;
double sampleRate;
int npoints ;
int acqPanel;
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements

int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Acquisition_H__ */
