/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/parsDefinition.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsDefinition.h,v 1.7.2.4 2009/07/08 08:47:00 dgem Exp $
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/




double parameter ExSliceGradient;
double parameter ExSliceRephaseGradient;
double parameter ExSliceBandWidth;
double parameter Phase3dGradient;
double parameter RfcSpoilerStrength;
double parameter RfcSpoilerDuration;
double parameter OneRepTime;

/* adjustment interface */
int parameter {relations AdjustmentCounterRels;} AdjustmentCounter;

double parameter
{
  display_name "Rephasing/encoding time";
  relations RephaseTimeRels;
  units "ms";
  format "%.2f";
} RephaseTime;


int parameter
{
  display_name "Number of Segments";
  relations NSegmentsRels;
} NSegments;

double parameter
{
  display_name "Echo Time";
  units "ms";
  format "%.3f";
  relations backbone;
} EchoTime;

double parameter
{
  display_name "Delay between volumes";
  format "%.2f";
  units "ms";
  relations PackDelRelation;
} PackDel;

double parameter
{
  display_name "Slice spoiler duration";
  format "%.2f";
  units "ms";
  relations backbone;
} SliceSpoilerDuration;

double parameter
{
  display_name "Slice spoiler strength";
  format "%.1f";
  units "%";
  relations backbone;
} SliceSpoilerStrength;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;

PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulseEnumRelation;
}RefPulseEnum;

PVM_RF_PULSE_TYPE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulseRelation;
}ExcPulse;

PVM_RF_PULSE_TYPE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulseRelation;
}RefPulse;

double parameter
{
  display_name "Bandwidth Scaling Factor";
  relations BwScaleRel;
  units "%";
  format "%f";
}BwScale;

int parameter
{
  display_name "Dummy scans";
  relations backbone;
} NDummyScans;

SIGNAL_TYPE parameter
{
  display_name "Signal Type";
  relations backbone;
} SignalType;

/********* HZeng      ***********/

char parameter
{
  display_name "Message";
} FP_MSG[256];

char parameter
{
  display_name "Configuration File";
  relations fpRelations;
} FP_config[256];

int parameter
{
  display_name "Number of Finger Pringting Scan";
} FP_NAcq;

double parameter
{
  display_name "Delay After Inversion (ms)";
} FP_delayInv;

double parameter
{
  display_name "Flip Angle List (degree)";
  units "deg";
} FP_flipAngleList[];

double parameter
{
  display_name "TE List (ms)";
  format "%.2f";
  units "ms";
} FP_TRList[];

double parameter varTRList[];
double parameter flipPowerList[];

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
