/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/parsLayout.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsLayout.h,v 1.13.2.3 2007/12/17 15:18:45 sako Exp $
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/
 /********* HZeng    ********/
 parclass
{
   FP_MSG;
   FP_config;
   FP_NAcq;
   FP_flipAngleList;
   FP_TRList;
}
attributes
{
   display_name "Finger Printing";
} fp;

parclass
{
  PVM_EffSWh;
  RephaseTime;
  PVM_MinFov;
  PVM_MinSliceThick;
  SliceSpoilerDuration;
  SliceSpoilerStrength;
  DigitizerPars;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

parclass
{
  BwScale;
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
  NDummyScans;

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_TaggingOnOff;
  Tagging_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_MagTransOnOff;
  Magn_Transfer_Parameters;

  PVM_FovSatOnOff;
  Sat_Slices_Parameters;

  PVM_TriggerOutOnOff;
  TriggerOut_Parameters;

} Preparation;


parclass
{
  PVM_EchoTime1;
  PVM_EchoTime;
  PVM_NEchoImages;
} ScanEditorInterface;


parclass
{
  Method;
  PVM_EffSWh;
  EchoTime;
  PVM_MinEchoTime;
  NSegments;
  PVM_RepetitionTime;
  PackDel;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  SignalType;
  PVM_DeriveGains;
  fp;
  Encoding;
  EPI_Parameters;
  RF_Pulses;
  Nuclei;
  Sequence_Details;
  StandardInplaneGeometry;
  StandardSliceGeometry;
  Preparation;
  Preemphasis;
  ScanEditorInterface;
  Method_RecoOptions;
} MethodClass;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
