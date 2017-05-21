/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/callbackDefs.h,v $
 *
 * Copyright (c) 1999-2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: callbackDefs.h,v 1.15.2.2 2007/09/07 09:45:28 dwe Exp $
 *
 ****************************************************************/

/* Encoding */
relations PVM_EncodingHandler backbone;

/* inplane geometry */
relations PVM_InplaneGeometryHandler  backbone;

/* slice geometry: */
relations PVM_SliceGeometryHandler localHandleSliceGeometry;

/* digitizer pars and bandwidth  */

relations PVM_DigHandler backbone;
relations PVM_EffSWh     EffSWhRel;

/* sequence atoms */
relations PVM_3dPhaseHandler   backbone;
relations PVM_2dPhaseHandler   backbone;
relations PVM_FreqEncodingHandler     backbone;
relations PVM_ExSliceSelectionHandler backbone;

/* modules */
relations PVM_FatSupHandler     backbone;
relations PVM_MagTransHandler   backbone;
relations PVM_SatSlicesHandler  backbone;
relations PVM_FlowSatHandler    backbone; 
relations PVM_TriggerHandler    backbone; 
relations PVM_EvolutionHandler  backbone; 
relations PVM_SelIrHandler      backbone; 
relations PVM_BlBloodHandler    backbone;

/* other parameters */
relations PVM_NucleiHandler    backbone;
relations PVM_DeriveGains      backbone;
relations PVM_EchoTime         backbone;
relations PVM_NAverages        Local_NAveragesHandler;
relations PVM_MotionSupOnOff   backbone;
relations PVM_RepetitionTime   backbone;
relations PVM_RareFactor       backbone;
relations PVM_ExcPulseAngle    RfcPulseAngleRelation;
relations PVM_FlipBackOnOff    backbone;
relations PVM_NRepetitions     backbone;

/* multi-echo parameters: */
relations PVM_EchoTime1        localEchoRels;
relations PVM_EchoTime2        backbone;
relations PVM_NEchoImages      backbone;
relations PVM_InversionTime    localInversionRel;

/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/






