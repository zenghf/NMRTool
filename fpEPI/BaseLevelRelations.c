/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/BaseLevelRelations.c,v $
 *
 * Copyright (c) 2001 - 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: BaseLevelRelations.c,v 1.18.2.10 2010/03/08 13:24:13 dgem Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: BaseLevelRelations.c,v 1.18.2.10 2010/03/08 13:24:13 dgem Exp $ (C) 2001 - 2009 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1

extern double MinTE_left, MinTE_right;

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



void SetBaseLevelParam( void )
{

  DB_MSG(("Entering SetBaseLevelParam()"));


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

  /* multiple receivers */
  if(Yes==ATB_SetMultiRec())
  {
    ATB_SetPulprog("fpEPI.4ch");
  }


  /* setting baselevel parameters used by modules */
  ATB_SetFatSupBaselevel();
  ATB_SetMagTransBaseLevel();
  ATB_SetSatSlicesBaseLevel();
  ATB_EpiSetBaseLevel();
  ATB_SetTriggerBaseLevel();
  ATB_SetTriggerOutBaseLevel();
  ATB_SetTaggingBaseLevel();

#if DEBUG
  printTiming();
#endif

  DB_MSG(("Exiting SetBaseLevelParam"));

}


/* Toolboxes referenced in this file: ATB, GTB, PTB, STB, UT */


void SetBasicParameters( void )
{
  int spatDim, specDim;
  int nSlices;
  int dim;

  DB_MSG(("Entering SetBasicParameters()"));
    
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

  /*
  ATB_SetAcqSize( Spatial, spatDim, PVM_EncMatrix, NULL, No );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }
  */
  ACQ_size[0] = PVM_EpiNSamplesPerScan*2;
  ACQ_size[1] = PVM_EpiNShots;
  if(spatDim>2)
    ACQ_size[2] = PVM_EncMatrix[2];

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


  /* NA */

  ATB_SetNA( PVM_NAverages );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }


  /* NAE */

  ATB_SetNAE( 1 );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }




  /* ACQ_ns */

  ACQ_ns_list_size = PVM_NEchoImages;
  
  dim = PARX_get_dim("ACQ_ns_list",1);
  if( dim != 1 )
    {
      PARX_change_dims( "ACQ_ns_list",1 );
    }

  NS = 1;
  ACQ_ns = NS;
  ACQ_ns_list[0] = ACQ_ns;

  ParxRelsParRelations("ACQ_ns",Yes);


  /* NECHOES */

   NECHOES = PVM_NEchoImages;



  /* ACQ_obj_order */

  PARX_change_dims("ACQ_obj_order",NI);

  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NEchoImages, 1 );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetBasicParameters: In function call!");
      return;
    }


  /* DS */

  DS = 0; /* L[0] = NDummyScans is directly used in ppg */
  ACQ_DS_enabled = No;

    
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


  DB_MSG(("Exiting SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{
  int nslices;

  DB_MSG(("Entering SetFrequencyParameters()"));

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

  ATB_SetRouting();

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



    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetFrequencyParameters: In function call!");
      return;
    }



  DB_MSG(("Exiting SetFrequencyParameters()"));
}

void SetGradientParameters( void )
{
  int spatDim, dim, i;

  DB_MSG(("Entering SetGradientParameters()"));
  

   ATB_SetAcqPhaseFactor( 1 );
  if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetGradientParameters: In function call!");
      return;
    }


  spatDim = PTB_GetSpatDim();

  dim = PARX_get_dim("ACQ_phase_encoding_mode", 1 );

  if( dim != spatDim )
    {
      PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
    }

  ACQ_phase_encoding_mode[0] = Read;
  ACQ_phase_encoding_mode[1] = Linear;
  if(spatDim==3)
  {
    ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
    ACQ_spatial_size_2 = PVM_EncMatrix[2];
    ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncValues2");
  }

  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);

  dim = PARX_get_dim("ACQ_phase_enc_start", 1 );

  if( dim != spatDim )
    {
      PARX_change_dims("ACQ_phase_enc_start", spatDim );
    }

  for( i=0; i<spatDim; i++ )
    {
      ACQ_phase_enc_start[i] = -1;
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

  ACQ_rare_factor = 1;

  ACQ_grad_str_X = 0.0;
  ACQ_grad_str_Y = 0.0;
  ACQ_grad_str_Z = 0.0;


   strcpy(GRDPROG, "");

   ATB_SetAcqTrims( 5,
		    ExSliceGradient,	         /* t0 */
		    (-ExSliceRephaseGradient),   /* t1 */
		    SliceSpoilerStrength,        /* t2 */
		    (SignalType==FID_signal ? -1.0:1.0)*Phase3dGradient,            /* t3 */
		    RfcSpoilerStrength           /* t4 */
   );
  
    if( PVM_ErrorDetected == Yes )
    {
      UT_ReportError("SetGradientParameters: In function call!");
      return;
    }



  DB_MSG(("Exiting SetGradientParameters()"));
}

void SetInfoParameters( void )
{
  int slices, i, spatDim, nrep;

  DB_MSG(("Entering SetInfoParameters()"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  spatDim = PTB_GetSpatDim();
  nrep = PVM_NRepetitions;

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

  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = EchoTime;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse.Length;

  /* calculation of ACQ_time_points */
  PARX_change_dims("ACQ_time_points",nrep);
  ACQ_time_points[0] = 0;
  for(i=1; i<nrep; i++)
    ACQ_time_points[i] = OneRepTime * i; 
  
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

  ACQ_n_echo_images = PVM_NEchoImages;
  ACQ_n_movie_frames = 1;
 
  DB_MSG(("Exiting SetInfoParameters()"));

}

void SetMachineParameters( void )
{
  DB_MSG(("Entering SetMachineParameters()"));

  /* setting of DIGMOD,DSPFIRM,AQ_mod done in frequency par setting routine */
 
  if( ParxRelsParHasValue("ACQ_word_size") == No )
    {
      ACQ_word_size = _32_BIT;
    }



  DE = DE < 6.0 ? 6.0: DE;

 
  PAPS = QP;

  ACQ_BF_enable = Yes;

  DB_MSG(("Exiting SetMachineParameters"));
}

void SetPpgParameters( void )
{
  DB_MSG(("Entering SetPpgParameters()"));

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


  ATB_SetPulprog("fpEPI.ppg");

  D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/NSLICES 
	   + 0.1) / 1000.0;
  D[1] = (RephaseTime - PVM_RiseTime) / 1000.0;
  if (SignalType == SPINECHO_signal)
    D[2]  = (EchoTime/2 - MinTE_right + PVM_InterGradientWaitTime) / 1000.0;
  else
    D[2] = (EchoTime - PVM_MinEchoTime + PVM_InterGradientWaitTime) / 1000.0;
  D[4]  = PVM_RiseTime / 1000.0;
  D[3]  = (PVM_RiseTime + PVM_InterGradientWaitTime) / 1000.0;
  D[5] = (SliceSpoilerDuration-PVM_RiseTime)*1e-3;
  D[6] = PackDel*1e-3;
  if(PackDel == 0.0) D[6] = 1*1e-6;
  D[7]  = (EchoTime/2 - MinTE_left + PVM_InterGradientWaitTime) / 1000.0;
  D[8] = CFG_AmplifierEnable()/1000.0;
  D[9] = (RfcSpoilerDuration-PVM_RiseTime)*1e-3;

 /* set shaped pulses, in this method TPQQ[0] is used           */


  sprintf(TPQQ[0].name,ExcPulse.Filename);
  if(PVM_DeriveGains == Yes)
    TPQQ[0].power  = ExcPulse.Attenuation;
  TPQQ[0].offset = 0.0;

  sprintf(TPQQ[1].name,RefPulse.Filename);
  if(PVM_DeriveGains == Yes)
    TPQQ[1].power  = RefPulse.Attenuation;
  TPQQ[1].offset = 0.0;

  ParxRelsParRelations("TPQQ",Yes);

  /* set duration of pulse, in this method P[0] is used          */


  P[0] = ExcPulse.Length * 1000;
  P[1] = RefPulse.Length * 1000; 
  ParxRelsParRelations("P",Yes);

  /* loop counters */
  L[0] = NDummyScans;
  L[2] = ACQ_dim>2? ACQ_size[2]:1;
  DB_MSG(("Exiting SetPpgParameters"));
}

#if DEBUG
#define d(n) D[n]*1e3
#define p(n) P[n]*1e-3

void printTiming(void)
{
  double prep, te,left_te,part1,part2,part3,tr;
  double trigger, trigOutSlice, trigOutVol;
  int nsl;

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

  DB_MSG(("-->printTiming"));

  prep=0.010+PVM_FovSatModuleTime + PVM_MagTransModuleTime + PVM_FatSupModuleTime; /* PVM_SatPulseModuleTime */ 
  part1=d(5)+d(4)+d(8)+p(0)+d(3)+d(1)+d(4);
  part2=d(7)+d(9)+d(4)+p(1)+d(9)+d(4);
  part3=d(2)+(PVM_EpiModuleTime)+0.01+d(0);

  nsl = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  left_te=ExcPulse.RephaseFactor/100*p(0)+d(3)+d(1)+d(4)+d(7)+d(9)+d(4)+p(1)/2.0;
  te=ExcPulse.RephaseFactor/100*p(0)+d(3)+d(1)+d(4)+d(2)+PVM_EpiEchoDelay;
  tr=prep+part1+part3;

  if (SignalType == SPINECHO_signal) {
    tr=tr+part2; 
    te=te+part2;  
    DB_MSG(("left TE: %fms, left TE (should be): %fms ",left_te, EchoTime/2.0));
  }

  tr=nsl*(trigger+trigOutSlice+tr)+d(6)+ PVM_TaggingModuleTime + trigOutVol +0.010;

  DB_MSG(("TR: %fms, TR (should be): %fms, diff: %fms",tr, PVM_RepetitionTime, (tr-PVM_RepetitionTime))); 
  DB_MSG(("TE: %fms, TE (should be): %fms, diff: %fms",te, EchoTime, (te-EchoTime)));


  DB_MSG(("Signal Type: %d,  ExcPulseLength %f, pvmrise %f, rephase %f, spoilerdur %f, refpulse %f  l %f r %f",
          SignalType, ExcPulse.Length, PVM_RiseTime, RephaseTime, RfcSpoilerDuration, RefPulse.Length, MinTE_left, MinTE_right ));  
  DB_MSG(("<--printTiming"));  
}

#undef d
#undef p
#endif
