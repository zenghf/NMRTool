/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/parsDefinition.h,v $
 *
 * Copyright (c) 1999-2001
 * BRUKER MEDICAL GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsDefinition.h,v 1.11.2.2 2008/04/01 12:58:18 sako Exp $
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/
double parameter ReadGradRatio;
double parameter SliceGradRatio;
double parameter Phase3dInteg;
double parameter Phase2dInteg;
double parameter minTE1;
double parameter minTE2;
double parameter EvolutionDuration;
double parameter OneRepTime;
double parameter
{
  display_name "Effective echo time";
  format "%.2f";
  units "ms";
  relations effTErels; 
} EffectiveTE[];


double parameter
{
  display_name "Slice Spoiler Duration";
  format "%.2f";
  units "ms";
  relations backbone;
} SliceSpoilerDuration;

double parameter
{
  display_name "Slice Spoiler Strength";
  format "%.1f";
  units "%";
  relations backbone;
} SliceSpoilerStrength;

double parameter
{
  display_name "Repetition Spoiler Duration";
  format "%.2f";
  units "ms";
  relations backbone;
} RepetitionSpoilerDuration;

double parameter
{
  display_name "Repetition Spoiler Strength";
  format "%.1f";
  units "%";
  relations backbone;
} RepetitionSpoilerStrength;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulseRelation;
}ExcPulse;

PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulseEnumRelation;
}RefPulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulseRelation;
}RefPulse;

double parameter
{
  display_name "Refocusing Flip Angle";
  format "%.1f";
  units "deg";
  relations RfcFlipAngleRel;
} RfcFlipAngle;

int parameter
{
  display_name "Number of Echoes";
} NEchoes;

double parameter
{
  display_name "Phase Encoding Start";
  format "%.2f";
  relations encodingRelations;
} PhaseEncodingStart;

int parameter 
{
  display_name "Number of Dummy Scans";
  relations dsRelations;
} NDummyScans;

double parameter
{
  display_name "Inter Slice Delay";
  relations backbone;
  units "ms";
  format "%.2f";
}SliceSegDur;

double parameter SliceSegDelay;
double parameter MinSliceSegDur;

double parameter SliceSegEndDelay;

/***JXU**/

double parameter
{
 display_name "Initial_MT_Offset";
 format "%5.0f";
 units "Hz";
 relations MTRels;
} Initial_MT_Offset;

double parameter
{
 display_name "Final_MT_Offset";
 format "%5.0f";
 units "Hz";
 relations MTRels;
} Final_MT_Offset;


int parameter
{
 display_name "Number_Offsets";
 format "%d";
 relations MTRels;
} Number_Offsets;


double parameter
{
 display_name "Offset_Step_Hz";
 format "%5.2f";
 units "Hz";
 relations MTRels;
} Offset_Step_Hz;

double parameter
{
 display_name "MT_Offsets";
 format "%5.0f";
 units "Hz";
 relations MTRels;
} MT_Offsets[];


LIST_MODE parameter
{
  display_name "MT Offset Calculation Mode";
  relations MTRels;
} MTOffsetMode;

int parameter
{
 display_name "M0 Offset";
 format "%d";
 units "Hz";
 relations MTRels;
} M0Offset;

int parameter
{
 display_name "Number of Pulses";
 relations MTRels;
} NPulse;

char parameter
{
  display_name "Freq. List in PPM";
  relations MTRels;
} MTListFile[256];


double parameter
{
 display_name "CEST Power uT";
 format "%3.2f";
 units "uT";
 relations MTRels;
} CEST_uT;


double parameter
{
 display_name "X Nuclei Ref. Att.";
 format "%5.1f";
 units "db";
 relations MTRels;
} CEST_attref;

PV_PULSE_LIST parameter
{
  display_name "CEST Pulse Shape";
  relations    MTRels;
} CESTPulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "CEST Pulse";
  relations    MTRels;
} CESTPulse;


double parameter
{
 display_name "Inter Pulse Delay";
 format "%5.1f";
 units "ms";
 relations MTRels;
} CEST_interdelay;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/




