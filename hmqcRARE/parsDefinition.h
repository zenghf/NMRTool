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



PV_PULSE_LIST parameter
{
  display_name "H 90 degree Pulse Shape";
  relations    H90PulseEnumRelation;
}H90PulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "H 90 degree Pulse";
  relations    H90PulseRelation;
}H90Pulse;

PV_PULSE_LIST parameter
{
  display_name "H 180 degree Pulse Shape";
  relations    H180PulseEnumRelation;
}H180PulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "H 180 degree Pulse";
  relations    H180PulseRelation;
}H180Pulse;

double parameter
{
  display_name "N15 Nuclei Ref. Att.";
  format "%5.1f";
  units "db";
  relations HMQCRels;
} N15_attref;


PV_PULSE_LIST parameter
{
  display_name "N15 Pulse Shape";
  relations    N15PulseEnumRelation;
}N15PulseEnum;


PVM_RF_PULSE_TYPE parameter
{
  display_name "N15 Pulse";
  relations    N15PulseRelation;
}N15Pulse;


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

/* Haifeng Zeng added HMQC parameters */
double parameter
{
  display_name "J coupling Time [1/2J]";
  format "%6.3f";
  units "ms";
  relations HMQCRels;
} dJCouple;

double parameter
{
  display_name "15N Evolution time [0.0]";
  format "%6.3f";
  units "ms";
  relations HMQCRels;
} dEvolve;


double parameter
{
  display_name "Exchange Time [60]";
  format "%6.3f";
  units "ms";
  relations HMQCRels;
} dExchange;


double parameter
{
  display_name "Spoiler Gradient [0.1]";
  format "%6.2f";
  relations HMQCRels;
} gradSpoiler;


HMQC_ACQ_ORDER_MODE parameter
{
  display_name "HMQC Acquisition Mode";
  relations HMQCRels;
} hmqcAcqOrder;

int parameter
{
  display_name "Number of HMQC transfer unit [30]";
  relations HMQCRels;
} nExchange;

int parameter
{
  display_name "Number of repetition";
  relations HMQCRels;
} nRepeat;

int parameter {
  display_name "ON OFF Option";
  relations HMQCRels;
} onOffOption[];
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/




