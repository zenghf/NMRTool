/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/initMeth.c,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: initMeth.c,v 1.17.2.8 2009/09/10 16:36:03 mawi Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: initMeth.c,v 1.17.2.8 2009/09/10 16:36:03 mawi Exp $ (C) Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for initialisation of the
 *	onepulse method.
 *
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History: 
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{
  int dimRange[2] = { 2,3 };
  int lowMat[3] = { 16, 16, 8 };
  int upMat[3]  = { 512, 512, 256 };
  int hasVal;

  DB_MSG(( "Entering epi2:initMeth()" ));

  /* which version of toolboxes should be active */
  PTB_VersionRequirement( Yes,20090101,"");
  
  /* initialize the Preemphasis group */
  STB_InitPreemphasis();
  
  /*  Initialize NA ( see code in parsRelations ) */
  Local_NAveragesRange();

  RephaseTimeRange();

  /* segments */
  NSegmentsRange();

  /* Encoding */
  STB_InitEncoding();

  /*Initialisation of repetitions time*/
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
      PVM_RepetitionTime = 1000;

  /*Initialisation of automatic derive RF gains*/
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
      PVM_DeriveGains = Yes;

  /* Initialisation of signal type */
  if(ParxRelsParHasValue("SignalType") == No)
      SignalType = FID_signal;


  /* Bandwidth Scaling Factor */
  BwScaleRange();

  /* Initialisation of rf pulse parameters */

  /* 1: flip angle in the scan edidor */
  if(ParxRelsParHasValue("PVM_ExcPulseAngle") == No)
      PVM_ExcPulseAngle = 90.0;
  ParxRelsShowInEditor("PVM_ExcPulseAngle");

  /* 2: pulses declared in parDefinitions.h 
     in this case - ExcPulse. We initalise it to default name, 
     1ms, and the flip angle given in PVM_ExcPulseAngle*/
  if(ParxRelsParHasValue("ExcPulse") == No)
  {
     STB_InitRFPulse(&ExcPulse,
		     CFG_RFPulseDefaultShapename(LIB_EXCITATION),
		     1.0,
		     PVM_ExcPulseAngle);
  }
  ExcPulseRange();

    if(ParxRelsParHasValue("RefPulse") == No)
  {
     STB_InitRFPulse(&RefPulse,
		     CFG_RFPulseDefaultShapename(LIB_REFOCUS),
		     1.0,
		     180.0);
  }
  RefPulseRange();

  /* 3: the corresponding pulse enums */
  STB_InitExcPulseEnum("ExcPulseEnum");
  STB_InitRfcPulseEnum("RefPulseEnum");

  /* Initialisation of nucleus and frequency */  
  STB_InitNuclei(1);


  /* Initialisation of the delay between the slices packages */  
  PackDelRange();
 
  /* Initialisation of spoilers */
  if(ParxRelsParHasValue("SliceSpoilerDuration") == No)
    SliceSpoilerDuration = 2;
  if(ParxRelsParHasValue("SliceSpoilerStrength") == No)
    SliceSpoilerStrength = 20;
  
  /* other parameters */
  if(ParxRelsParHasValue("NDummyScans") == No)
    NDummyScans = 0;


  /* Initialisation of geometry parameters */
  /* A: in-plane */

  hasVal=ParxRelsParHasValue("PVM_Matrix");

  STB_InitStandardInplaneGeoPars(2,dimRange,lowMat,upMat,No);

  if(hasVal == 0) /* forcing an init matrix of 64x64 */
  {
    PVM_Matrix[0] = PVM_Matrix[1] = 64;
  }

  /* B: slice geometry */

  STB_InitSliceGeoPars(0,0,0);
 
  
  /* initializtion of bandwidth */
  LocalSWhRange();
  STB_InitDigPars();

  
  /* not a csi experiment */
  PTB_SetSpectrocopyDims( 0, 0 );

  /* Initialisation of modules */
  STB_InitEpi(UserSlope, No_navigators);
  STB_InitFatSupModule();
  STB_InitMagTransModule();
  STB_InitSatSlicesModule();
  STB_InitTriggerModule();
  STB_InitTriggerOutModule();
  STB_InitTaggingModule();

  /* Visibility of Scan Editor parameters */
  ParxRelsShowInEditor("PVM_EchoTime1,PVM_EchoTime,PVM_NEchoImages");
  ParxRelsHideClassInEditor("ScanEditorInterface");
  ParxRelsShowInFile("PVM_EchoTime1,PVM_EchoTime,PVM_NEchoImages");
  ParxRelsMakeNonEditable("PVM_EchoTime1,PVM_EchoTime,PVM_MinEchoTime");
  ParxRelsMakeNonEditable("PVM_NEchoImages");


  /* Once all parameters have initial values, the backbone is called
     to assure they are consistent */
  backbone();
 

  DB_MSG(( "Exiting epi2:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









