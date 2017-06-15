/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/config/proto.head,v $
 *
 * Copyright (c) 1995
 * BRUKER ANALYTISCHE MESSTECHNIK GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $State: Exp $
 *
 *******************************************************************
 */

#ifndef _P_
#	if defined(HAS_PROTO) || defined(__STDC__) || defined(__cplusplus)
#		define _P_(s) s
#	else
#		define _P_(s) ()
#	endif
#endif

/* /home/nmrsu/methods/src/csRARERandEnc/initMeth.c */
void initMeth _P_((void));
/* /home/nmrsu/methods/src/csRARERandEnc/loadMeth.c */
void loadMeth _P_((const char *));
/* /home/nmrsu/methods/src/csRARERandEnc/parsRelations.c */
void backbone _P_((void));
void RfcPulseAngleRelation _P_((void));
void RfcFlipAngleRel _P_((void));
void ExcPulseEnumRelation _P_((void));
void ExcPulseRelation _P_((void));
void RefPulseEnumRelation _P_((void));
void RefPulseRelation _P_((void));
void localEchoRels _P_((void));
void effTErels _P_((void));
void dsRelations _P_((void));
void dsRange _P_((void));
void ExcPulseRange _P_((void));
void RefPulseRange _P_((void));
void echoTimeRels _P_((void));
void SliceSegDurRels _P_((void));
void repetitionTimeRels _P_((void));
void LocalGeometryMinimaRels _P_((double *, double *));
void LocalGradientStrengthRels _P_((void));
void LocalFrequencyOffsetRels _P_((void));
void Local_NAveragesRange _P_((void));
void Local_NAveragesHandler _P_((void));
void encodingRelations _P_((void));
void localHandleSliceGeometry _P_((void));
void EffSWhRange _P_((void));
void EffSWhRel _P_((void));
void localInversionRel _P_((void));
void ConstrainGradientLimits _P_((YesNo));
void csRelations _P_((void));
void initCSRel _P_((void));
void setupCSEnc _P_((void));
void readConfig _P_((void));
void readEnc _P_((void));
/* /home/nmrsu/methods/src/csRARERandEnc/BaseLevelRelations.c */
void SetBaseLevelParam _P_((void));
void SetBasicParameters _P_((void));
void SetFrequencyParameters _P_((void));
void SetGradientParameters _P_((void));
void SetInfoParameters _P_((void));
void SetMachineParameters _P_((void));
void SetPpgParameters _P_((void));
/* /home/nmrsu/methods/src/csRARERandEnc/RecoRelations.c */
void SetRecoParam _P_((void));
int PowerOfTwo _P_((int));
