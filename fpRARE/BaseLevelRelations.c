/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/BaseLevelRelations.c,v $
 *
 * Copyright (c) 2002-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: BaseLevelRelations.c,v 1.27.2.10 2009/09/09 16:00:49 mawi Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: BaseLevelRelations.c,v 1.27.2.10 2009/09/09 16:00:49 mawi Exp $(C) 2002-2009 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"

void SetBaseLevelParam( void )
{

  DB_MSG(("-->SetBaseLevelParam\n"));
  
  SetBasicParameters();
  
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: Error in function call!");
    return;
  }
  
  SetFrequencyParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  SetPpgParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  SetGradientParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  SetInfoParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }
  
  
  SetMachineParameters();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBaseLevelParam: In function call!");
    return;
  }


  /* ------------------------------------------------------------------------
     Sets parameters needed for multi-receiver acq. Overrides some previously
     set parameters such as NUCn Must be called at the end of
     SetBaseLevel.
     ----------------------------------------------------------------------- */
  if(Yes==ATB_SetMultiRec())
  {
    ATB_SetPulprog("fpRARE.4ch");
  }
  
  /* setting baselevel parameters used by modules */
  ATB_SetFatSupBaselevel();
  ATB_SetMagTransBaseLevel();
  ATB_SetSatSlicesBaseLevel();
  ATB_SetFlowSaturationBaseLevel();
  ATB_SetTriggerBaseLevel();
  ATB_SetEvolutionBaseLevel();
  ATB_SetBlBloodBaseLevel();
  {
  int nSlices;

  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_SetSelIrBaseLevel(nSlices);
  }

#if DEBUG
  printTiming();
#endif

  DB_MSG(("<--SetBaseLevelParam\n"));
  
}





void SetBasicParameters( void )
{
  int spatDim, specDim;
  int nSlices;
  int echo;
  
  DB_MSG(("-->SetBasicParameters\n"));
  
  /* ACQ_dim */
  
  spatDim = PTB_GetSpatDim();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  specDim = PTB_GetSpecDim();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  ACQ_dim = spatDim + specDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  /* ACQ_dim_desc */
  
  ATB_SetAcqDimDesc( specDim, spatDim, NULL );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* ACQ_size */

  /* With the Encoding group, this call
     ATB_SetAcqSize( Spatial, spatDim, PVM_Matrix, PVM_AntiAlias, No );
     is replaced by: */
  ATB_SetAcqSize( Spatial, spatDim, PVM_EncMatrix, NULL, No );
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* NSLICES */
  
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  ATB_SetNSlices( nSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* NR */
  
  ATB_SetNR( PVM_NRepetitions );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  /* NI */
  
  ATB_SetNI( nSlices * PVM_NEchoImages );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  
  /* AVERAGING */


  switch(PVM_MotionSupOnOff)
  {
  default:
  case Off:
    ATB_SetNA( PVM_NAverages );
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }
    ATB_SetNAE( 1 );
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }
    break;
  case On:
    ATB_SetNAE( PVM_NAverages );
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }
    ATB_SetNA( 1 );
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }
    break;
  }
  
  
  /* ACQ_ns */
  
  ACQ_ns_list_size = PVM_NEchoImages;

  ParxRelsParRelations("ACQ_ns_list_size",Yes);
  for(echo=0; echo<PVM_NEchoImages; echo++)
    ACQ_ns_list[echo] = 1;
  NS = ACQ_ns = ACQ_ns_list[0];
  
  
  
  /* NECHOES */
  
  NECHOES = NEchoes;
  
  
  /* ACQ_obj_order */
  
  PARX_change_dims("ACQ_obj_order",NI);
  
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NEchoImages, 1 );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  /* DS */
  
  DS = NDummyScans;
  ACQ_DS_enabled = Yes;
  
  
  ATB_DisableAcqUserFilter();
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
      return;
  }

  ATB_SetAcqScanSize( One_scan );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetBasicParameters: In function call!");
    return;
  }
  
  
  DB_MSG(("<--SetBasicParameters\n"));
}

void SetFrequencyParameters( void )
{
  int nslices;
  
  DB_MSG(("-->SetFrequencyParameters\n"));
  
  
  ATB_SetNuc1(PVM_Nucleus1);
  
  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");
  
  ATB_SetNucleus(PVM_Nucleus1);
  
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  /* setting of SW_h, DIGMOD, DSPFIRM and AQ_mod */

  ATB_SetDigPars();

  
  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode",Yes);
  
  ACQ_O2_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O2_mode",Yes);
  
  ACQ_O3_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O3_mode",Yes);
  
  O1 = 0.0;
  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;
  
  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);
  
  
  ATB_SetRouting();
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }
  
  
  
  DB_MSG(("<--SetFrequencyParameters\n"));
}

void SetGradientParameters( void )
{
  int spatDim, dim;

  DB_MSG(("-->SetGradientParameters\n"));
  
  
  ATB_SetAcqPhaseFactor( PVM_RareFactor );
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetGradientParameters: In function call!");
    return;
  }
  
  
  spatDim = PTB_GetSpatDim();
  
  dim = PARX_get_dim("ACQ_phase_encoding_mode", 1 );
  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );
  switch(spatDim)
  {
    case 3:
      ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
      ACQ_phase_enc_start[2] = -1; /* set, but no used */
      ACQ_spatial_size_2 = PVM_EncMatrix[2];
      ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncValues2");
      /* no break */
    case 2:
      ACQ_phase_encoding_mode[1] = User_Defined_Encoding;;
      ACQ_phase_enc_start[1] = -1.0; /* set, but no used */
      ACQ_spatial_size_1 = PVM_EncMatrix[1];
      ParxRelsCopyPar("ACQ_spatial_phase_1","PVM_EncValues1");
      /* no break */
    default:
      ACQ_phase_encoding_mode[0] = Read;
      ACQ_phase_enc_start[0] = -1;
  }

   
  ATB_SetAcqGradMatrix( PVM_NSPacks, PVM_SPackArrNSlices,
			PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_ObjOrderList );
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetGradientParameters: In function call!");
    return;
  }
  
  
  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  ACQ_rare_factor = PVM_RareFactor;
  
  ACQ_grad_str_X = 0.0;
  ACQ_grad_str_Y = 0.0;
  ACQ_grad_str_Z = 0.0;
  
  strcpy(GRDPROG, "");
  
  ATB_SetAcqTrims( 11,
                   PVM_ExSliceGradient,	          /* t0 */
                   -PVM_ExSliceRephaseGradient,   /* t1 */
                   PVM_ExSliceGradient,           /* t2 */
                   SliceSpoilerStrength,          /* t3 */
                   PVM_ReadDephaseGradient,       /* t4 */
                   PVM_ReadGradient,              /* t5 */
                   PVM_2dPhaseGradient,           /* t6 */
                   -PVM_3dPhaseGradient,          /* t7 */
                   -PVM_2dPhaseGradient,          /* t8 */
                   PVM_3dPhaseGradient,           /* t9 */
                   RepetitionSpoilerStrength      /* t10 */
                   );
  
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetGradientParameters: In function call!");
    return;
  }
  
  DB_MSG(("<--SetGradientParameters\n"));
}

void SetInfoParameters( void )
{
  int slices, i, k, spatDim, evol, nrep;

  DB_MSG(("-->SetInfoParameters\n"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();
  
  spatDim = PTB_GetSpatDim();
  evol = PVM_NEvolutionCycles;
  nrep = PVM_NEvolutionCycles*PVM_NRepetitions;

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetInfoParameters: In function call!");
    return;
  }
  
  ATB_SetAcqMethod();
  
  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetInfoParameters: In function call!");
    return;
  }
  
  ACQ_flip_angle = PVM_ExcPulseAngle;

  ACQ_n_echo_images = PVM_NEchoImages;
  ACQ_n_movie_frames = 1;
  
  PARX_change_dims("ACQ_echo_time",PVM_NEchoImages);
  for(i=0; i<PVM_NEchoImages; i++)
    ACQ_echo_time[i] = EffectiveTE[i];
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - 
	PVM_EchoTime * ((double)PVM_RareFactor - 0.5);

  /* calculation of ACQ_time_points */
  PARX_change_dims("ACQ_time_points",nrep);
  ACQ_time_points[0] = 0;
  if(PVM_EvolutionMode == Constant_Delay)
  {
    for(i=1; i<nrep; i++)
      ACQ_time_points[i] = (OneRepTime + PVM_EvolutionDelay) * i; 
  }
  else
  {
    for(i=1; i<evol; i++)
	ACQ_time_points[i] = ACQ_time_points[i-1] + PVM_EvolutionTime[i-1]+ OneRepTime;
    
    if(PVM_NRepetitions >1)
    {
      for(k=1; k<PVM_NRepetitions; k++)
      {
	ACQ_time_points[k*evol] = ACQ_time_points[k*evol-1] + OneRepTime;
      
	for(i=1; i<evol; i++)
	  ACQ_time_points[k*evol+i] = ACQ_time_points[k*evol+i-1] + PVM_EvolutionTime[i-1]+ OneRepTime;
      }    
    }
  }
  
  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;
  
  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );
  
  ACQ_slice_orient = Arbitrary_Oblique;
  
  ACQ_slice_thick = PVM_SliceThick;
  
  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetInfoParameters: In function call!");
    return;
  }
  
  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);
  
  for(i=0;i<slices;i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }

  ACQ_read_ext = (int)PVM_AntiAlias[0];
  
  PARX_change_dims("ACQ_slice_sepn", slices==1 ? 1 : slices-1);
  
  if( slices == 1 )
  {
    ACQ_slice_sepn[0] = 0.0;
  }
  else
  {
    for( i=1; i<slices;i++ )
    {
      ACQ_slice_sepn[i-1]=PVM_SliceOffset[i]-PVM_SliceOffset[i-1];
    }
  }
  
  ATB_SetAcqSliceSepn( PVM_SPackArrSliceDistance,
                       PVM_NSPacks );
  
  
  
  ATB_SetAcqPatientPosition();
  
  ATB_SetAcqExpType( Imaging );
  
  ACQ_n_t1_points = 1;
  
  if( ParxRelsParHasValue("ACQ_transmitter_coil") == No )
  {
    ACQ_transmitter_coil[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_contrast_agent") == No )
  {
    ACQ_contrast_agent[0] = '\0';
  }

  if( ParxRelsParHasValue("ACQ_contrast") == No )
  {
    ACQ_contrast.volume = 0.0;
    ACQ_contrast.dose = 0.0;
    ACQ_contrast.route[0] = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0] = '\0';
  }
  
  ParxRelsParRelations("ACQ_contrast_agent",Yes);
  
  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;
  
  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;
  
  ACQ_RF_power = 0;
  
  ACQ_flipback = No;
  
  DB_MSG(("<--SetInfoParameters\n"));
  
}

void SetMachineParameters( void )
{
  double dval;
  DB_MSG(("-->SetMachineParameters\n"));

  
  if( ParxRelsParHasValue("ACQ_word_size") == No )
  {
    ACQ_word_size = _32_BIT;
  }
  

  dval = PVM_GradDelayTime + PVM_2dPhaseGradientTime - 0.01;
  dval = MAX_OF(PVM_DigEndDelOpt,dval);

  DEOSC = (PVM_AcquisitionTime  + dval)*1000.0;
  ACQ_scan_shift = -1;
  ParxRelsParRelations("ACQ_scan_shift",Yes);
  
  DE = DE < 6.0 ? 6.0: DE;
  PAPS = QP;
  
  ACQ_BF_enable = Yes;
  
  DB_MSG(("<--SetMachineParameters\n"));
}

void SetPpgParameters( void )
{
  DB_MSG(("-->SetPpgParameters\n"));
  
  if( ParxRelsParHasValue("ACQ_trigger_enable") == No )
  {
    ACQ_trigger_enable = No;
  }
  
  if( ParxRelsParHasValue("ACQ_trigger_reference") == No )
  {
    ACQ_trigger_reference[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_trigger_delay") == No )
  {
    ACQ_trigger_delay = 0;
  }
  
  ParxRelsParRelations("ACQ_trigger_reference",Yes);
  
  
  ACQ_vd_list_size=1;
  PARX_change_dims("ACQ_vd_list",1);
  ACQ_vd_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vd_list",Yes);
  
  ACQ_vp_list_size=1;
  PARX_change_dims("ACQ_vp_list",1);
  ACQ_vp_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vp_list",Yes);
  
  
  ATB_SetPulprog("fpRARE.ppg");
  if(PVM_SelIrOnOff)
  {
    D[0] = PVM_InterGradientWaitTime / 1000.0;
    D[10] = SliceSegEndDelay/1000.0;
  }
  else
  {
    D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/NSLICES 
	   + PVM_InterGradientWaitTime) / 1000.0;
    D[10] = 0.00001;
  }
  D[1]  = ((PVM_EchoTime - minTE1)/2.0 + PVM_InterGradientWaitTime + PVM_GradDelayTime) / 1000.0;
  D[2]  = ((PVM_EchoTime - minTE2)/2.0 +0.04 + PVM_InterGradientWaitTime) / 1000.0; /* 0.04 is duration of ADC_END_4ch */
  D[3]  = PVM_GradDelayTime / 1000.0;
  D[3]  = MAX_OF(D[3], 0.0);  
  D[4]  = PVM_RampTime / 1000.0;
  D[5]  = (SliceSpoilerDuration - 2*PVM_RampTime)/1000.0;
  D[6]  = (PVM_2dPhaseGradientTime - 2*PVM_RampTime)/1000.0;
  D[11] = (PVM_ReadDephaseTime - 2.0*PVM_RampTime) / 1000.0;
  D[8] = CFG_AmplifierEnable()/1000.0;
  D[9] = (RepetitionSpoilerDuration - 2*PVM_RampTime)/1000.0;
 
  /* set shaped pulses     */
  sprintf(TPQQ[0].name,ExcPulse.Filename);
  sprintf(TPQQ[1].name,RefPulse.Filename);
  if(PVM_DeriveGains == Yes)
  {
    TPQQ[0].power  = ExcPulse.Attenuation;
    TPQQ[1].power  = RefPulse.Attenuation;
  }
  TPQQ[0].offset = 0.0;
  TPQQ[1].offset = 0.0;
  
  ParxRelsParRelations("TPQQ",Yes);
  
  /* set duration of pulse, in this method P[0] is used          */
  P[0] = ExcPulse.Length * 1000;
  P[1] = RefPulse.Length * 1000;
  ParxRelsParRelations("P",Yes);
  
  /* counters used in pulse program */
  L[1] = NDummyScans*PVM_RareFactor;
  L[2] = (PVM_EncMatrix[1])/PVM_RareFactor;
  
  DB_MSG(("<--SetPpgParameters\n"));
}


#if DEBUG
#define d(n) D[n]*1e3
#define p(n) P[n]*1e-3

void printTiming(void)
{
  double te1,te2,aqq=PVM_DigDur, tr;


  /* 90 - 180 */
  te1 = p(0)/2 + d(4) + 0.01 + d(4) + d(11) + d(4) + d(1) + d(4) + d(5) + d(4) + d(3) + p(1)/2;

  /* 180 - 180 */
  te2 = (p(1)/2+d(3)+d(4)+d(5)+d(4)+d(2)+d(4)+d(6)+d(4)+d(3)+aqq/2 )*2;
  DB_MSG(("te1: %f should be %f diff %f\n"
	  "te2: %f should be %f diff %f\n",
	  te1,PVM_EchoTime/2,te1-PVM_EchoTime/2,
	  te2,PVM_EchoTime,te2-PVM_EchoTime));

  /* TR */
  tr = 0.01 + d(4)+d(9)+d(4)+d(3)+d(8)+p(0)/2+                   /* before mid-90 */ 
       te1 +                                                     /* mid90 - mid180 */
                                                                 /* mid180 till end (1 echo):*/
       p(1)/2 + d(3)+d(4)+d(5)+d(4) + 2*( d(2)+d(4)+d(6)+d(4)+d(3)) + aqq + 
       (NECHOES-1) * te2  +                                      /* additional echoes */
       d(0);

  if(PVM_FlipBackOnOff == On)
  {
    double tail; /* sequence under "if" in ppg */
 
    tail = 2*(d(4)+d(5)+d(4)+d(3)) + p(1) + (d(1)-d(3)) + d(4)+d(11)+d(4) + 0.01 + d(4)+d(3)+p(0)+d(4);

    tr+= tail;
  }

  tr *= NSLICES;

  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif
