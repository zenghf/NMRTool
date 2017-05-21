/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/RecoRelations.c,v $
 *
 * Copyright (c) 1999 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Locker:  $
 * $State: Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      0

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *      of this method. This function will be automatically
 *      called in response to a method file for this method
 *      having been read.
 *
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*              I N T E R F A C E   S E C T I O N               */
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*              I N C L U D E   F I L E S                       */
/****************************************************************/


#include "method.h"

void SetRecoParam(int acceleration )

{

  int dim,i, k, slice, nslices, ftSize[3];

  DB_MSG(("Entering SetRecoParam"));

  /* set baselevel reconstruction parameter */
  /* default initialization of reco based on acqp pars allready set */

  ATB_InitDefaultReco();

  dim = PTB_GetSpatDim();

  for(i=0; i<dim; i++)
    ftSize[i] = (int)(PVM_Matrix[i]*PVM_AntiAlias[i]);

  /* select method specific reconstruction method */
  RECO_mode = USER_MODE;
  ParxRelsParRelations("RECO_mode",Yes);
  ATB_InitUserModeReco(ACQ_dim, PVM_EncMatrix, ftSize, 
		       PVM_EncSteps1, PVM_EncSteps2,
		       PVM_EncNReceivers, 
		       acceleration, 
		       24 /* reference lines */,
		       NI, ACQ_obj_order, PVM_EpiNEchoes, 
		       50 /* echo position in read */);

  /* set scaling values for phased array coils */
  for(k=0; k<PVM_EncNReceivers;k++)
    RecoScaleChan[k] = PVM_EncChanScaling[k];
  
  RECO_inp_order = NO_REORDERING; 
  RECO_regrid_mode = NO_REGRID;   
  ParxRelsParRelations("RECO_regrid_mode",Yes);

  for(i=0; i<dim; i++)
    RECO_bc_mode[i] = NO_BC;


  RecoAutoDerive = No;
  
  /* set reco sizes and ft_mode (dim 2&3) */
  for(i=0; i<dim; i++)
  {
    RECO_ft_mode[i] = (ftSize[i] == PowerOfTwo(ftSize[i])) ?  COMPLEX_FFT:COMPLEX_FT;
    RECO_inp_size[i] = ftSize[i];
    RECO_ft_size[i] = ftSize[i];
    RECO_size[i] = PVM_Matrix[i];
    for(k=0; k<NI; k++)
      RECO_offset[i][k] = (RECO_ft_size[i]-RECO_size[i])/2;
  }
  
  /* fov offset in read */
  nslices = PARX_get_dim("PVM_EffReadOffset",1);
  for(k=0; k<NI; k++)
  {
    slice = (k / PVM_NEchoImages)%nslices;
    RECO_offset[0][k] -= (int)(RECO_ft_size[0] * PVM_EffReadOffset[slice]
                                                / (PVM_Fov[0]*PVM_AntiAlias[0]));   
  }

  /* set reco rotate according to phase offsets     */


  ATB_SetRecoRotate(PVM_EffPhase1Offset,
                    PVM_Fov[1]*PVM_AntiAlias[1],
                    NSLICES,     
                    1,           /* 1 echo image    */
                    1) ;         /* phase1 direction*/

  if(dim==3)
  {

    ATB_SetRecoRotate(PVM_EffPhase2Offset, 
		      PVM_Fov[2]*PVM_AntiAlias[2],
		      NSLICES,     
		      1,           /* 1 echo image    */
		      2) ;         /* phase2 direction*/

  }


  for(i=0;i<dim;i++)
    RECO_fov[i]= PVM_FovCm[i];
  
  ParxRelsParRelations("RECO_fov",Yes);
  
  ATB_SetRecoTransposition(PtrType3x3 ACQ_grad_matrix[0],
                           PVM_NSPacks,
                           NSLICES,
                           ACQ_ns_list_size,
                           ACQ_obj_order) ;
  
  
  DB_MSG(("Exiting SetRecoParam"));
  
}

/* Relation of RecoUserUpdate, called at start of reconstruction
   to set reconstrution process */
 
void DeriveReco(void)
{

  DB_MSG(("--> DeriveReco ==================="));
  
  ATB_EpiSetRecoProcess(PVM_EncNReceivers, 
			PVM_EncMatrix[0], 
			(int)(PVM_Matrix[0]*PVM_AntiAlias[0]), 
			PVM_EncCentralStep1);

  DB_MSG(("<--  DeriveReco =================="));
}

int PowerOfTwo(int x)
/* returns next power of two */
{

 return (1<<(int)ceil(log((double)x)/log(2.0)));
}
