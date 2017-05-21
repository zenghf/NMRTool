/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/parsLayout.h,v $
 *
 * Copyright (c) 1999-2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsLayout.h,v 1.16.2.3 2007/12/07 14:48:23 sako Exp $
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

parclass
{
  PVM_EffSWh;
  PVM_ReadDephaseTime;
  PVM_2dPhaseGradientTime;
  PVM_MinEchoTime;
  PVM_MinFov;
  PVM_MinSliceThick;
  SliceSpoilerDuration;
  SliceSpoilerStrength;
  RepetitionSpoilerDuration;
  RepetitionSpoilerStrength;
  PhaseEncodingStart;
  DigitizerPars;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

parclass
{
  PVM_SliceBandWidthScale;
  ExcPulseEnum;
  ExcPulse;
  RefPulseEnum;
  RefPulse;
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

parclass
{ 
  CESTPulseEnum;
  CESTPulse; 
  CEST_attref;
  CEST_uT;
  CEST_interdelay;
  NPulse;
  Number_Offsets;
  MT_Offsets;
  MTOffsetMode;
  MTListFile;
  Initial_MT_Offset;
  Final_MT_Offset;
  Offset_Step_Hz;
}
attributes
{
  display_name "Cest Parameters";
}Cest_Parameters;

parclass
{

  NDummyScans;

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_EvolutionOnOff;
  Evolution_Parameters;

  PVM_SelIrOnOff;
  Selective_IR_Parameters;

  PVM_BlBloodOnOff;
  BlackBlood_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
 
 
  PVM_FovSatOnOff;
  Sat_Slices_Parameters;
 
  PVM_InFlowSatOnOff;
  Flow_Sat_Parameters;

  PVM_MotionSupOnOff;
  
  PVM_FlipBackOnOff;
 
} Preparation;

/* The following class is defined to assure that parameters of the 
 * scan editor are properly displayed. */
parclass
{
  PVM_EchoTime1;
  PVM_EchoTime2;
} ScanEditorInterface;

parclass
{
  Method;
  PVM_EchoTime;
  EffectiveTE;
  PVM_RareFactor;
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_NEchoImages;
  PVM_ScanTimeStr;
  RfcFlipAngle;
  PVM_DeriveGains;
  Cest_Parameters;
  RF_Pulses;
  Nuclei;
  Encoding;
  Sequence_Details;
  StandardInplaneGeometry;
  StandardSliceGeometry;
  Preparation;
  ScanEditorInterface;
  Method_RecoOptions;
} MethodClass;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



