/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/parsRelations.c,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsRelations.c,v 1.31.2.15 2010/03/19 07:48:35 dgem Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: parsRelations.c,v 1.31.2.15 2010/03/19 07:48:35 dgem Exp $ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *	of this method. This function will be automatically
 *	called in response to a method file for this method
 *	having been read.
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

double  MinTE_left, MinTE_right;

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/

double  Lim3dPhaseGradient;
double  LimExSliceGradient;
double  LimExSliceRephaseGradient;
double  SliceGradRatio;
double  Phase3dInteg;
double  Phase2dInteg;
double  ReadGradient;

/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/* ------------------------------------------------------------
  backbone
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{
  YesNo refAttIsAvailable=No;
  int dim, ret, nImagesPerRep;
  double referenceAttenuation=0,
         minFov[3] = {1e-3, 1e-3, 1e-3},
         minThickness;



  DB_MSG(("Entering EPI/parsRelations.c:backbone"));

  STB_DefaultUserTypeHandler(); /* this controls param visibility
				   for expert/routine */

  /* update of preemphasis; must take place before nuclei update */
  STB_UpdatePreemphasis();

  /* Nuclei and  PVM_GradCalConst  are handled by this funtion: */
  STB_UpdateNuclei(Yes);

  PVM_UserType = Expert_User;

  /* handle RF pulse */

  if(PVM_DeriveGains == Yes)
    refAttIsAvailable =
      STB_GetRefAtt(1,PVM_Nucleus1,&referenceAttenuation);
  else
    refAttIsAvailable = No;

  STB_UpdateRFPulse("ExcPulse",
		    &ExcPulse,
		    refAttIsAvailable,
		    referenceAttenuation);

  STB_UpdateExcPulseEnum("ExcPulseEnum",
			 &ExcPulseEnum,
			 ExcPulse.Filename,
			 ExcPulse.Classification);

  PVM_ExcPulseAngle = ExcPulse.FlipAngle;

  STB_UpdateRFPulse("RefPulse",
		    &RefPulse,
		    refAttIsAvailable,
		    referenceAttenuation);

  STB_UpdateRfcPulseEnum("RefPulseEnum",
			 &RefPulseEnum,
			 RefPulse.Filename,
			 RefPulse.Classification);

  ExSliceBandWidth = ExcPulse.Bandwidth * BwScale/100.0;

  /* signal type */
  if(SignalType != SPINECHO_signal)
  {
    ParxRelsHideInEditor("RefPulse,RefPulseEnum,BwScale");
  }
  else
  {
    ParxRelsShowInEditor("RefPulse,RefPulseEnum,BwScale");
  }

  /* spoilers */
  SliceSpoilerStrength = MIN_OF(SliceSpoilerStrength, 100.0);
  SliceSpoilerStrength = MAX_OF(SliceSpoilerStrength, -100.0);
  SliceSpoilerDuration = MAX_OF(SliceSpoilerDuration, 2*PVM_RiseTime);

  /* sequence variables */
  Lim3dPhaseGradient = 50;
  LimExSliceGradient = 100;
  LimExSliceRephaseGradient = 50;


   /*** begin Update Geometry: **/

  /* 1: in-plane geometry:
      The STB_StandardInplaneGeoParHandler is called
      first to make size constraints;
  */

  /* do not allow isotropic geometry */
  PVM_Isotropic = Isotropic_None;

  /* do not allow a-aliasing */
  PVM_AntiAlias[1] = 1.0;

  STB_StandardInplaneGeoParHandler(minFov,2.0);
  LocalGeometryMinimaRels(minFov, &minThickness);

  /** 2: slice geometry:*/

  ConstrainReadOffset(); /* special for EPI */

  dim = PTB_GetSpatDim();

  if(dim == 3)
  {
    double min;

    min=MAX_OF(minFov[2],minThickness);
    minFov[2] = minThickness = min;
    PVM_SliceThick = PVM_Fov[2];
    /*  constrain maximum slices per package to 1 */
    STB_UpdateSliceGeoPars(0,1,1,minThickness);
  }
  else
  {
    /* no constrain to slices in 2D mode*/
    STB_UpdateSliceGeoPars(0,0,0,minThickness);
  }

  /*** end Update Geometry **/

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  PVM_NEchoImages = 1;
  nImagesPerRep = PVM_NEchoImages * GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );


  /*** encoding */
  /* note: Grappa reference lines are disabled. Grappa coeeficients will be set
   * in a special adjustment. */

  STB_UpdateEncodingForEpi(PTB_GetSpatDim(),  /* total dimensions */
		     PVM_Matrix,        /* image size */
		     PVM_AntiAlias,     /* a-alias */
		     &NSegments,        /* segment size */
 		     Yes,               /* ppi in 2nd dim allowed */
		     No,                /* ppi ref lines in 2nd dim allowed */
		     Yes);              /* partial ft in 2nd dim allowed */

   /** Update EPI parameter group */

  ret = STB_EpiUpdate(dim,
		      PVM_EncMatrix,
		      PVM_AntiAlias,
		      PVM_Fov,
		      minFov,
		      &PVM_EffSWh,
		      PVM_GradCalConst,
		      nImagesPerRep,
		      PVM_NRepetitions,
		      NSegments,
		      PVM_EncCentralStep1,
		      PVM_EncPpiAccel1,
		      PVM_EncNReceivers);

  if(ret <0)
    DB_MSG(("--!!!!!!!!!!!!!!! Illegal arguments for STB_UpdateEPI: EPI Module not ready !"));

  /* minFov is now known; we update geometry again */
  STB_StandardInplaneGeoParHandler(minFov,2.0);


  LocalGradientStrengthRels();
  LocalFrequencyOffsetRels();

  if (ParxRelsParHasValue("PVM_NAverages") == 0)
     PVM_NAverages = 1;

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1);
  STB_UpdateMagTransModule();
  STB_UpdateSatSlicesModule(PVM_Nucleus1);
  STB_UpdateTriggerModule();
  STB_UpdateTriggerOutModule();
  STB_UpdateTaggingModule(PVM_Nucleus1,PVM_Fov,PVM_Matrix);

  dummyScansRels();

  rfcSpoilerUpdate();

  echoTimeRels();

  repetitionTimeRels();

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam(PVM_EncPpiAccel1);


  /* adapt size of trajectory arrays if necessary and set
     PVM_EpiTrajAdj to No if a trajectory relevant parameter has changed */

  STB_EpiCheckTrajectory(PVM_Fov[0],PVM_EncMatrix[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);

  SetAdjustmentRequests();  /* PVM_EpiTrajAdjMeasured needs to be known */

  DB_MSG(("Exiting EPI/parsRelations.c:backbone"));
}


void rfcSpoilerUpdate(void)
{
  RfcSpoilerStrength = 2*ExSliceGradient;
  RfcSpoilerStrength = MIN_OF(RfcSpoilerStrength,80);
  RfcSpoilerStrength = MAX_OF(RfcSpoilerStrength,30);

  RfcSpoilerDuration = MAX_OF(RefPulse.Length, 2*PVM_RiseTime);
}

/* Range checking and relation of Bandwidth Scaling Factor */

void BwScaleRange(void)
{
  DB_MSG(("-->BwScaleRange\n"));

  if(ParxRelsParHasValue("BwScale")==No)
  {
    BwScale=100.0;
  }
  else
  {
    BwScale=MAX_OF(MIN_OF(150,BwScale),50.0);
  }


  DB_MSG(("<--BwScaleRange\n"));
  return;
}

void BwScaleRel(void)
{
  DB_MSG(("-->BwScaleRel\n"));
  BwScaleRange();
  backbone();
  DB_MSG(("<--BwScaleRel\n"));
  return;
}

/*--------------------------------------------------------------
  ExcPulseAngleRelation
  Redirected relation of PVM_ExcPulseAngle
  -------------------------------------------------------------*/
void ExcPulseAngleRelation(void)
{
  DB_MSG(("-->ExcPulseAngleRelation"));
  ExcPulse.FlipAngle = PVM_ExcPulseAngle;
  ExcPulseRange();
  backbone();
  DB_MSG(("<--ExcPulseAngleRelation"));
}



/* --------------------------------------------------------------
   ExcPulseEnumRelation

   --------------------------------------------------------------*/
void ExcPulseEnumRelation(void)
{
  YesNo status;
  DB_MSG(("-->ExcPulsesEnumRelation"));

  /* set the name and clasification of ExcPulse: */
  status = STB_UpdateExcPulseName("ExcPulseEnum",
				  &ExcPulseEnum,
				  ExcPulse.Filename,
				  &ExcPulse.Classification);

  /* call the method relations */
  backbone();

  DB_MSG(("<--ExcPulseEnumRelation status = %s",
	  status == Yes? "Yes":"No"));
}



/* -----------------------------------------------------------
   Relation of ExcPulse

   -----------------------------------------------------------*/
void ExcPulseRelation(void)
{
  DB_MSG(("-->ExcPulseRelation"));

  /* Tell the request handling system that the parameter
     ExcPulse has been edited */
  UT_SetRequest("ExcPulse");

  /* Check the values of ExcPulse */
  ExcPulseRange();

  /* call the backbone; further handling will take place there
     (by means of STB_UpdateRFPulse)  */

  backbone();

  DB_MSG(("-->ExcPulseRelation"));
}

void ExcPulseRange(void)
{
  DB_MSG(("-->ExcPulseRange"));

  /* allowed clasification */

  switch(ExcPulse.Classification)
  {
  default:
    ExcPulse.Classification = LIB_EXCITATION;
    break;
  case LIB_EXCITATION:
  case PVM_EXCITATION:
  case USER_PULSE:
    break;
  }

  /* allowed angle for this pulse */

  ExcPulse.FlipAngle = MIN_OF(90.0,ExcPulse.FlipAngle);


  /* general verifiation of all pulse atributes  */

  STB_CheckRFPulse(&ExcPulse);

  DB_MSG(("<--ExcPulseRange"));

}

/* --------------------------------------------------------------
   Relation of RefPulseEnum

   --------------------------------------------------------------*/
void RefPulseEnumRelation(void)
{
  YesNo status;
  DB_MSG(("-->RefPulsesEnumRelation"));

  /* set the name and clasification of ExcPulse: */
  status = STB_UpdateRfcPulseName("RefPulseEnum",
				  &RefPulseEnum,
				  RefPulse.Filename,
				  &RefPulse.Classification);

  /* call the method relations */
  backbone();

  DB_MSG(("<--RefPulseEnumRelation status = %s",
	  status == Yes? "Yes":"No"));
}


/* -----------------------------------------------------------
   Relation of RefPulse

   -----------------------------------------------------------*/
void RefPulseRelation(void)
{
  DB_MSG(("-->RefPulseRelation"));

  /* Tell the request handling system that the parameter
     RefPulse has been edited */
  UT_SetRequest("RefPulse");

  /* Check the values of RefPulse */
  RefPulseRange();

  /* call the backbone; further handling will take place there
     (by means of STB_UpdateRFPulse)  */

  backbone();

  DB_MSG(("-->RefPulseRelation"));
}

/* --------------------------------------------------------------
   Range checking of RefPulse

   --------------------------------------------------------------*/
void RefPulseRange(void)
{
  DB_MSG(("-->RefPulseRange"));

  /* allowed clasification */

  switch(RefPulse.Classification)
  {
  default:
    RefPulse.Classification = LIB_REFOCUS;
    break;
  case LIB_REFOCUS:
  case PVM_REFOCUS:
  case USER_PULSE:
    break;
  }

  /* allowed angle for this pulse */

  RefPulse.FlipAngle = MIN_OF(180.0,RefPulse.FlipAngle);


  /* general verifiation of all pulse atributes  */

  STB_CheckRFPulse(&RefPulse);

  DB_MSG(("<--RefPulseRange"));

}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/


void dummyScansRels()
/* ------------------------------------------------------
   constraining DS
------------------------------------------------------- */
{
  NDummyScans = MAX_OF(NDummyScans, 0);
}


void echoTimeRels( void )
{
  DB_MSG(("Entering EPI/parsRelations.c:echoTimeRels()"));

  if(SignalType == FID_signal)
  {
    PVM_MinEchoTime =      /* min gradient echo time */
      ExcPulse.Length * ExcPulse.RephaseFactor/100 +
      PVM_RiseTime               +
      PVM_InterGradientWaitTime  +
      RephaseTime       +
      PVM_InterGradientWaitTime  +
      PVM_EpiEchoDelay;
    MinTE_right  = MinTE_left = 0.0; /* not used */
  }
  else
  {
    MinTE_left =  /* min half spinecho-time given by left hand side of pi */
    ExcPulse.Length * ExcPulse.RephaseFactor/100  +
    PVM_RiseTime               +
    PVM_InterGradientWaitTime  +
    RephaseTime       +
    PVM_InterGradientWaitTime  +
    RfcSpoilerDuration +
    RefPulse.Length/2.0;

    MinTE_right = /* min half spinecho-time given by right hand side of pi */
    RefPulse.Length/2.0 +
    RfcSpoilerDuration +
    PVM_InterGradientWaitTime  +
    PVM_EpiEchoDelay;

    PVM_MinEchoTime = 2 * MAX_OF(MinTE_left, MinTE_right);
  }

  EchoTime = EchoTime < PVM_MinEchoTime ?
    PVM_MinEchoTime : EchoTime;


  /* Set Echo Parameters for Scan Editor   */
  PVM_EchoTime = PVM_Matrix[0]*PVM_DigDw;  /* echo spacing */
  PVM_EchoTime1 = EchoTime;

  DB_MSG(("Exiting EPI/parsRelations.c:echoTimeRels"));
}

void repetitionTimeRels( void )
{
  int nSlices,dim;
  double TotalTime,trigger, trigger_v, trigOutSlice, trigOutVol;
  double amplifierenable;

  DB_MSG(("--> minRepetitionTimeRels"));

  trigger = STB_UpdateTriggerModule();
  if(PVM_TriggerMode == per_PhaseStep) /* per volume */
  {
    trigger_v=trigger;
    trigger=0.0;
  }
  else trigger_v=0.0;

  amplifierenable = CFG_AmplifierEnable();
  TotalTime = 0.0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("EPI backbone: In function call!");
      return;
    }

  if(PVM_TriggerOutOnOff == On)
  {
    switch(PVM_TriggerOutMode)
    {
    case PER_SLICE:
      trigOutSlice = PVM_TriggerOutModuleTime;
      trigOutVol = 0.0;
      break;

    case PER_VOLUME:
      trigOutSlice = 0.0;
      trigOutVol = PVM_TriggerOutModuleTime;
      break;

    case AT_START:
    default:
      trigOutSlice = 0.0;
      trigOutVol = 0.0;

    }
  }
  else
    trigOutSlice = trigOutVol = 0.0;

  dim = PTB_GetSpatDim();

  if(dim>2) /* disable inter-volume delay in 3d */
  {
    PackDel=0;
    ParxRelsMakeNonEditable("PackDel");
  }
  else
  {
    ParxRelsMakeEditable("PackDel");
  }


  PVM_MinRepetitionTime =
    nSlices * ( 0.01 +
               PVM_FatSupModuleTime +
               PVM_MagTransModuleTime +
               PVM_FovSatModuleTime +
               trigger +
               trigOutSlice +
               SliceSpoilerDuration +
               amplifierenable +
               ExcPulse.Length/2 +
               EchoTime +
               PVM_EpiModuleTime - PVM_EpiEchoDelay +
               0.01 + /* 10u */
               0.1 /* 100u: min d0, includes islice */
    ) +
    PVM_TaggingModuleTime +
    trigOutVol +
    trigger_v +
    PackDel +
    0.010;

  if(PackDel==0.0) PVM_MinRepetitionTime+=0.001;

  PVM_RepetitionTime = ( PVM_RepetitionTime < PVM_MinRepetitionTime ?
			 PVM_MinRepetitionTime : PVM_RepetitionTime );

  /** Calculate Total Scan Time and Set for Scan Editor **/

  if( dim >1 )
    TotalTime = PVM_RepetitionTime*PVM_EpiNShots*PVM_NAverages*PVM_NRepetitions;
  if( dim >2 )
    TotalTime *= PVM_EncMatrix[2];

  /* time for one repetition */
  OneRepTime = TotalTime/(PVM_NRepetitions*1000.0);

  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);


  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels"));
}

void LocalGeometryMinimaRels(double *min_fov, double *min_thickness )
{
  /*
    This function calculates the minima for the field-of-view and the minimum
    slice thickness. It is always assumed that all slices have the same thickness


    The results is returned in min_fov[2] and min_thickness.
    (WE DO NOT set min_fov[0 and 1] here, this will be done by the epi module)
 */

  double sliceRampInteg; /* normalised integral falling slice gradient ramp */
  double sliceRephInteg; /* normalised integral slice rephase gradient      */

  if( PTB_GetSpatDim() == 3)
  {

      /*
	Step #1:
	Calculate the normalised integral of the phase encoding gradient
	in the 3rd dimension
      */
      Phase3dInteg = RephaseTime - PVM_RiseTime;
      /*
	Step #2:
	Calculate the resulting minimum field-of-view in that direction
      */
      min_fov[2] = MRT_PhaseFov( Phase3dInteg,
				 PVM_Matrix[2],
				 Lim3dPhaseGradient,
				 PVM_GradCalConst );
  }


  /* min slice thickness: */
  /*  Calculate the normalised integral of the descending gradient ramp after
      the RF pulse */
  sliceRampInteg = 0.5 *  PVM_RiseTime;
  /* Calculate the normalised integral of the slice selection rephasing
     gradient */
  sliceRephInteg = RephaseTime - PVM_RiseTime;;

  /*
	Calculate the ratio of the strength of the slice selection gradient to
	the strength of the slice selection rephase gradient

  */
  SliceGradRatio = MRT_SliceGradRatio( ExcPulse.Length,
                                       ExcPulse.RephaseFactor,
                                       0.0,
                                       sliceRampInteg,
                                       sliceRephInteg );
  /*
    Calculate the minimum slice thickness
  */

  *min_thickness = MRT_MinSliceThickness( ExSliceBandWidth,
					  SliceGradRatio,
					  LimExSliceGradient,
					  LimExSliceRephaseGradient,
					  PVM_GradCalConst );


} /* end of localGeometryMinima() */



void LocalGradientStrengthRels( void )
{
  /*
    This function calculates all the gradient strengths
  */
  PVM_3dPhaseGradient = 0.0;

  switch( PTB_GetSpatDim() )
    {
    case 3: /* PHASE ENCODING GRADIENT - 3nd DIM */
      /*
	Calculate the strength of the 3d phase encoding gradient
       */
      Phase3dGradient = MRT_PhaseGrad( Phase3dInteg,
					   PVM_Matrix[2],
					   PVM_Fov[2],
					   PVM_GradCalConst );
     /* falls through */
    case 1:
      ReadGradient = (PVM_EpiReadEvenGrad+PVM_EpiReadOddGrad)/2.0; /* used in LocFreqOff */
     /* falls through */
    default: /* SLICE SELECTION GRADIENT */
      ExSliceGradient = MRT_SliceGrad( ExSliceBandWidth,
				       PVM_SliceThick,
				       PVM_GradCalConst );
      ExSliceRephaseGradient = MRT_SliceRephaseGrad( SliceGradRatio,
							 ExSliceGradient );
      break;
    }
}

void LocalFrequencyOffsetRels( void )
{
  int spatDim;
  int i,nslices;

  spatDim = PTB_GetSpatDim();

  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  /* set ReadOffsetHz to zero. In EPI the fov offset is made by reco, not by detection freq. offsets */
  MRT_FrequencyOffsetList(nslices,
			  PVM_EffReadOffset,
			  ReadGradient,
			  0.0, /* instead PVM_GradCalConst; this sets offHz to zero */
			  PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  ExSliceGradient,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_SliceOffset[i];
  }


}


void PackDelRange( void )
{
  if(ParxRelsParHasValue("PackDel") == No)
    PackDel = 0.0;
  PackDel = MAX_OF(PackDel,0.0);
}

void PackDelRelation( void )
{
  PackDelRange();
  backbone();
}

void RephaseTimeRels(void)
{
  RephaseTimeRange();
  backbone();
}

void RephaseTimeRange(void)
{
  if(!ParxRelsParHasValue("RephaseTime"))
    RephaseTime = 2.0;

  RephaseTime = MAX_OF(RephaseTime, 2*CFG_GradientRiseTime());
}

void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("Entering Local_NAveragesRange"));

  /*
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
    {
      PVM_NAverages = 1;
    }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);


  DB_MSG(("Exiting Local_NAveragesRange"));


}



void Local_NAveragesHandler(void)
{

  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));

  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */


  backbone();


  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));
  return;
}
void LocalSWhRange(void)
{
  DB_MSG(("-->LocalSWhRange"));

  if(!ParxRelsParHasValue("PVM_EffSWh"))
  {
    PVM_EffSWh = 100000.0;
  }
  else
  {
    PVM_EffSWh = MAX_OF(50000.0,PVM_EffSWh);
    PVM_EffSWh = MIN_OF(1000000.0,PVM_EffSWh);
  }

  DB_MSG(("<--LocalSWhRange"));
}

void LocalSWhRels(void)
{
  DB_MSG(("-->LocalSWhRel"));

  UT_SetRequest("PVM_EffSWh");
  LocalSWhRange();
  backbone();

  DB_MSG(("<--LocalSWhRel"));
  return;
}


/*
 *  local function to constrain the read offset for slice packages
 */

void ConstrainReadOffset(void)
{
  int dim=0,i=0;
  double *offs, max;
  DB_MSG(("-->ConstrainReadOffset"));

  dim = PARX_get_dim("PVM_SPackArrReadOffset",1);
  offs = PVM_SPackArrReadOffset;

  max = (PVM_Fov[0]/2)*(PVM_AntiAlias[0]-1.0);
  max = MAX_OF(max, 0);

  for(i=0;i<dim;i++)
  {
    offs[i]=MAX_OF(offs[i], -max);
    offs[i]=MIN_OF(offs[i], max);
  }


  DB_MSG(("<--ConstrainReadOffset"));
  return;
}


/* relations of NSegments */

void NSegmentsRels(void)
{
  NSegmentsRange();
  backbone();
}

void NSegmentsRange(void)
{
  if(!ParxRelsParHasValue("NSegments"))
    NSegments = 1;
  else
    NSegments = MAX_OF(1,NSegments);
}

void localHandleSliceGeometry(void)
{
  DB_MSG(("-->localHandleSliceGeometry\n"));

  if(PTB_GetSpatDim()==3)
  {
    PVM_Fov[2] = PVM_SliceThick;
  }

  backbone();

  DB_MSG(("<--localHandleSliceGeometry\n"));
  return;
}


/*HZeng fp init and relation check*/
void fpRelations(void)
{
  initFPRel();
  readFPRel();
  backbone();
}

void initFPRel(void) {
  printf("----> initFPRel\n");

  strcpy(FP_MSG, "Please Select a config file");
  ParxRelsMakeNonEditable("CS_MSG, FP_NAcq, FP_delayInv, FP_flipAngleList, FP_TRList");
  FP_NAcq = 1;
  //FP_delayInv = (d4*2) - d3 - d8 - d9 - (p2 * 0.5) - (p0 * 0.5);
  FP_delayInv = 0.1;
  PARX_change_dims("FP_flipAngleList", FP_NAcq);
  PARX_change_dims("FP_TRList", FP_NAcq);
  FP_flipAngleList[0] = 90.0;
  FP_TRList[0] = PVM_EchoTime;

  printf("<---- initFPRel\n");
}

/* calculate varTRList, flipPowerList */
void calcFPRel(void) {
  printf("----> calcFPRel");
  PARX_change_dims("varTRList", FP_NAcq);
  PARX_change_dims("flipPowerList", FP_NAcq);
  int i = 0;
  for (i = 0; i < FP_NAcq; i++) {
    varTRList[i] = FP_TRList[i] - PVM_EchoTime;
    flipPowerList[i] = log10(90.0 / FP_flipAngleList[i]) * 20 + ExcPulse.Attenuation;
    /* do note take cortab into account */
  }
  printf("<---- calcFPRel");
}


void readFPRel(void)
{
  printf("----> calcCSRel\n");
  int i;
  char line [1024], str1[1024];
  FILE *fd;
  int n;
  double num1, num2;

  printf("read FPFile\n");
  fd = fopen(FP_config, "r");
  if (fd != NULL) {
    printf("open FPFile successfully!\n");
    char s = '#';
    while (s == '#') {
      fgets(line, 1024, fd);
      s = line[0];
    }

    // read FP_NAcq
    sscanf(line, "%s %d", str1, &n);
    FP_NAcq = n;
    PARX_change_dims("FP_flipAngleList", FP_NAcq);
    PARX_change_dims("FP_TRList", FP_NAcq);

    // read FP_delayInv
    fgets(line, 1024, fd);
    sscanf(line, "%s %lf", str1, &num1);
    FP_delayInv = num1;

    // read FP_flipAngleList, FP_TRList
    i = 0;
    while( fgets(line, 1024, fd) != NULL && i < FP_NAcq) {
      sscanf(line, "%lf %lf", &num1, &num2);
      FP_flipAngleList[i] = num1;
      FP_TRList[i] = num2;
      i++;
    }
    sprintf(FP_MSG, "Successfully Load %s", FP_config);
    fclose(fd);
    calcFPRel();
  }
  else
  {
    printf("fail to open FPFile!\n");
  }
  printf("<---- calcCSRel\n");
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
