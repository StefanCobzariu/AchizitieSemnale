/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREC_PANEL                       1       /* callback function: OnFrecPanelCB */
#define  FREC_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  FREC_PANEL_TEXTMSG               3       /* control type: textMsg, callback function: (none) */
#define  FREC_PANEL_IDC_GRAPH_SPECTRU     4       /* control type: graph, callback function: (none) */
#define  FREC_PANEL_IDC_NUMAR_PUNCTE      5       /* control type: ring, callback function: (none) */
#define  FREC_PANEL_POWER_PEAK_IDC        6       /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_FREC_PEAK_IDC         7       /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_IDC_GRAPH_WINDOW      8       /* control type: graph, callback function: (none) */
#define  FREC_PANEL_IDC_GRAPH_FILTRED     9       /* control type: graph, callback function: (none) */
#define  FREC_PANEL_FILTER_TYPE           10      /* control type: ring, callback function: (none) */
#define  FREC_PANEL_IDC_APPLY_FILTER      11      /* control type: command, callback function: Filter_CB */
#define  FREC_PANEL_TIP_FEREASTRA         12      /* control type: ring, callback function: (none) */
#define  FREC_PANEL_IDC_APPLY_FREC_BUTTON 13      /* control type: command, callback function: FrequencyCB */
#define  FREC_PANEL_IDC_GRAPH_FREC_RAW    14      /* control type: graph, callback function: (none) */
#define  FREC_PANEL_START_FREC_IDC        15      /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_STOP_FREC_IDC         16      /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_FREC_NEXT_IDC         17      /* control type: command, callback function: FREC_NEXT_CB */
#define  FREC_PANEL_FREC_PREV_IDC         18      /* control type: command, callback function: FREC_PREV_CB */
#define  FREC_PANEL_IDC_N_POINTS          19      /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_IDC_SAMPLERATE        20      /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_FPASS_IDC             21      /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_FCUT_IDC              22      /* control type: numeric, callback function: (none) */
#define  FREC_PANEL_GRAPH_SIG_RAW_WINDOW  23      /* control type: graph, callback function: (none) */
#define  FREC_PANEL_IDC_GRAPH_FIL_SPECTRU 24      /* control type: graph, callback function: (none) */
#define  FREC_PANEL_IDC_SALVARE_IMAGINI   25      /* control type: command, callback function: CB_SALVARE_IMAGINI */

#define  WAVE_PANEL                       2       /* callback function: OnWavePanelCB */
#define  WAVE_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  WAVE_PANEL_COMMANDBUTTON         3       /* control type: command, callback function: OnLoadButtonCB */
#define  WAVE_PANEL_IDC_GRAPH_RAW_DATA    4       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_IDC_GRAPH_FILTERED    5       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_START_IDC             6       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_STOP_IDC              7       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_NEXT_IDC              8       /* control type: command, callback function: NEXT_CB */
#define  WAVE_PANEL_PREV_IDC              9       /* control type: command, callback function: PREV_CB */
#define  WAVE_PANEL_FILTER                10      /* control type: ring, callback function: selectFilter */
#define  WAVE_PANEL_APLICA                11      /* control type: command, callback function: APLICA_CB */
#define  WAVE_PANEL_MIN                   12      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MAX                   13      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MEAN                  14      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_VARIANCE              15      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_GRAPH_HISTOGRAM   16      /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_NZERO                 17      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_NELEMENTE_IDC         18      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_ALPHA_IDC             19      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_DECORATION            20      /* control type: deco, callback function: (none) */
#define  WAVE_PANEL_IDC_ANVELOPA          21      /* control type: command, callback function: ANVELOPA_CB */
#define  WAVE_PANEL_IDC_ASIMETRIA         22      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_APLATIZAREA       23      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_MAX_INDEX         24      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_MIN_INDEX         25      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_IDC_MEDIAN            26      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ANVELOPA_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK APLICA_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_SALVARE_IMAGINI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Filter_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FREC_NEXT_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FREC_PREV_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FrequencyCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NEXT_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrecPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnWavePanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PREV_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK selectFilter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif