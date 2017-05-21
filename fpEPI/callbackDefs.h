/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/EPI/callbackDefs.h,v $
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: callbackDefs.h,v 1.8.2.6 2010/02/08 10:57:55 dgem Exp $
 *
 ****************************************************************/


/* digitizer group */
relations PVM_DigHandler backbone;

/* inplane geometry */
relations PVM_InplaneGeometryHandler  backbone;

/* slice geometry: */
relations PVM_SliceGeometryHandler  localHandleSliceGeometry;

/* preemphasis */
relations PVM_PreemphasisHandler backbone;

/* bandwidth */
relations PVM_EffSWh            LocalSWhRels;

/* modules */
relations PVM_FatSupHandler     backbone;
relations PVM_MagTransHandler   backbone;
relations PVM_SatSlicesHandler  backbone;
relations PVM_FlowSatHandler    backbone;
relations PVM_TriggerHandler    backbone;
relations PVM_EpiHandler        backbone;
relations PVM_TriggerOutHandler backbone;
relations PVM_TaggingHandler    backbone;

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_NRepetitions      backbone;
relations PVM_ExcPulseAngle     ExcPulseAngleRelation;

/* reconstruction */
relations RecoUserUpdate        DeriveReco;
relations VisuDerivePars        deriveVisu;

/* encoding group */
relations PVM_EncodingHandler   backbone;

/* Adjustment related redirections */
relations PVM_AdjResultHandler   HandleAdjustmentResults;
relations PVM_AdjHandler         HandleAdjustmentRequests;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







