/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/initMeth.c,v $
 *
 * Copyright (c) 2002-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: initMeth.c,v 1.23.2.5 2009/09/10 16:25:22 mawi Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: initMeth.c,v 1.23.2.5 2009/09/10 16:25:22 mawi Exp $(C) 2002-2004 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"

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
  int lowMat[3]   = { 32, 32, 8 };
  int upMat[3]    = { 2048, 2048, 256 };


  DB_MSG(("-->initMeth\n"));

  /* which version of toolboxes should be active */
  PTB_VersionRequirement( Yes,20090101,"");
  
 
  
  /*  Initialize NA ( see code in parsRelations ) */
  Local_NAveragesRange();
  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;
  /* init. dummy scans */
  dsRange();

  /* 
   * Which parameter classes (see parDefs.h) should be
   * hidden in routine mode 
   */

  PTB_SetUserTypeClasses( "Nuclei,"
			  "Sequence_Details,"
			  "RF_Pulses" );

  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 1000.0;
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 20.0;
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  /* Initialisation of rf pulse parameters */

  /* 
   * 1: flip angle in the scan edidor 
   * (use for refoc. pulse angle in this method)
   */

  ParxRelsShowInEditor("PVM_ExcPulseAngle");
  

  /* 
   * 2: pulses declared in parDefinitions.h 
   * in this case - ExcPulse and RefPulse. 
   */

  if(ParxRelsParHasValue("ExcPulse") == No)
    STB_InitRFPulse(&ExcPulse,
		    CFG_RFPulseDefaultShapename(LIB_EXCITATION),
		    1.0,  /* default duration in ms */
		    90.0);
  ExcPulseRange();
  if(ParxRelsParHasValue("RefPulse") == No)
    STB_InitRFPulse(&RefPulse,
		    CFG_RFPulseDefaultShapename(LIB_REFOCUS),
		    1.0, /* default duration in ms */
		    180.0 );
  RefPulseRange();
  
  
  /* 3: the corresponding pulse enums */
  STB_InitExcPulseEnum("ExcPulseEnum");
  STB_InitRfcPulseEnum("RefPulseEnum");
  

  /* Initialisation of nucleus */  
  STB_InitNuclei(1);
  
  
  /* Initialisation of spoilers */
  
  if(ParxRelsParHasValue("SliceSpoilerDuration") == No)
    SliceSpoilerDuration = 1;
  if(ParxRelsParHasValue("SliceSpoilerStrength") == No)
    SliceSpoilerStrength = 20;
  if(ParxRelsParHasValue("RepetitionSpoilerDuration") == No)
    RepetitionSpoilerDuration = 2;
  if(ParxRelsParHasValue("RepetitionSpoilerStrength") == No)
    RepetitionSpoilerStrength = 30;
  
 
  /* 
   * Initialisation of geometry parameters 
   * A: in-plane 
   */

  STB_InitStandardInplaneGeoPars(2,dimRange,lowMat,upMat,No);
  
  /* B: slice geometry */

  STB_InitSliceGeoPars(0,0,0);

  /* 
   * set gradient limits according to desired obliqueness
   */
  ConstrainGradientLimits(PVM_MajSliceOri);
  
  /* Initialisation of atoms */

  if(ParxRelsParHasValue("PVM_ReadDephaseTime") == No)
    PVM_ReadDephaseTime = 3.0;
  if(ParxRelsParHasValue("PVM_2dPhaseGradientTime") == No)
    PVM_2dPhaseGradientTime = 2.0;
  STB_InitReadAtoms();  
  STB_InitExSliceAtoms();
  STB_Init2dPhaseAtoms();
  STB_Init3dPhaseAtoms();

  /* initialize digitizer parameter */

  STB_InitDigPars();
  EffSWhRange();

  /* Encoding */
  STB_InitEncoding();

  /* phase encoding start is defined by new encoding group */
   ParxRelsResetPar("PhaseEncodingStart");
  
  /* echo parameters */
  
  if(ParxRelsParHasValue("PVM_RareFactor") == No)
    PVM_RareFactor = 8;
  if(ParxRelsParHasValue("PVM_NEchoImages") == No)
    PVM_NEchoImages = 1;
  if(ParxRelsParHasValue("PVM_FlipBackOnOff") == No)
    PVM_FlipBackOnOff = Off;
  PVM_NEchoScan1 = 1;
  PVM_NEchoScan2 = 1;
  ParxRelsShowInEditor("PVM_EchoTime1,PVM_EchoTime2");
  ParxRelsMakeNonEditable("PVM_MinEchoTime");
  ParxRelsMakeNonEditable("NEchoes,PVM_NEchoScan1,PVM_NEchoScan2");
  ParxRelsMakeEditable("PVM_FlipBackOnOff");
  ParxRelsShowInEditor("PVM_FlipBackOnOff");
  ParxRelsShowInEditor("PVM_EchoTime1,PVM_EchoTime2");
  ParxRelsHideClassInEditor("ScanEditorInterface");
  ParxRelsShowInFile("PVM_EchoTime1,PVM_EchoTime2");

  /* PhaseEncodingStart only used to convert old protocols */
  ParxRelsHideInEditor("PhaseEncodingStart");
  ParxRelsHideInFile("PhaseEncodingStart");
  
  /* not a csi experiment */
  PTB_SetSpectrocopyDims( 0, 0 );
  
  /* activating motion suppression */
  if (ParxRelsParHasValue("PVM_MotionSupOnOff") == 0)
     PVM_MotionSupOnOff = Off;
  PARX_hide_pars(NOT_HIDDEN,"PVM_MotionSupOnOff"); 
   

  /* Initialisation of modules */
  STB_InitFatSupModule();
  STB_InitMagTransModule();
  STB_InitSatSlicesModule();
  STB_InitFlowSaturationModule();
  STB_InitTriggerModule();
  STB_InitEvolutionModule();
  STB_InitSelIrModule();
  STB_InitBlBloodModule();  
  
  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */
  

  backbone();

  DB_MSG(( "<--initMeth\n" ));

}




/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









