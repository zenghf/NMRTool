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

/* /home/nmrsu/methods/src/fpEPI/initMeth.c */
void initMeth _P_((void));
/* /home/nmrsu/methods/src/fpEPI/loadMeth.c */
void loadMeth _P_((const char *));
/* /home/nmrsu/methods/src/fpEPI/parsRelations.c */
void backbone _P_((void));
void rfcSpoilerUpdate _P_((void));
void BwScaleRange _P_((void));
void BwScaleRel _P_((void));
void ExcPulseAngleRelation _P_((void));
void ExcPulseEnumRelation _P_((void));
void ExcPulseRelation _P_((void));
void ExcPulseRange _P_((void));
void RefPulseEnumRelation _P_((void));
void RefPulseRelation _P_((void));
void RefPulseRange _P_((void));
void dummyScansRels _P_((void));
void echoTimeRels _P_((void));
void repetitionTimeRels _P_((void));
void LocalGeometryMinimaRels _P_((double *, double *));
void LocalGradientStrengthRels _P_((void));
void LocalFrequencyOffsetRels _P_((void));
void PackDelRange _P_((void));
void PackDelRelation _P_((void));
void RephaseTimeRels _P_((void));
void RephaseTimeRange _P_((void));
void Local_NAveragesRange _P_((void));
void Local_NAveragesHandler _P_((void));
void LocalSWhRange _P_((void));
void LocalSWhRels _P_((void));
void ConstrainReadOffset _P_((void));
void NSegmentsRels _P_((void));
void NSegmentsRange _P_((void));
void localHandleSliceGeometry _P_((void));
void fpRelations _P_((void));
void initFPRel _P_((void));
void calcFPRel _P_((void));
void readFPRel _P_((void));
/* /home/nmrsu/methods/src/fpEPI/BaseLevelRelations.c */
void SetBaseLevelParam _P_((void));
void SetBasicParameters _P_((void));
void SetFrequencyParameters _P_((void));
void SetGradientParameters _P_((void));
void SetInfoParameters _P_((void));
void SetMachineParameters _P_((void));
void SetPpgParameters _P_((void));
/* /home/nmrsu/methods/src/fpEPI/RecoRelations.c */
void SetRecoParam _P_((int));
void DeriveReco _P_((void));
int PowerOfTwo _P_((int));
/* /home/nmrsu/methods/src/fpEPI/adjust.c */
void SetAdjustmentRequests _P_((void));
void HandleAdjustmentRequests _P_((void));
void AdjustmentCounterRels _P_((void));
void HandleAdjustmentResults _P_((void));
void setRGGhostAdj _P_((YesNo));
void setSpinEcho _P_((void));
void setTrajAdj _P_((void));
/* /home/nmrsu/methods/src/fpEPI/deriveVisu.c */
void deriveVisu _P_((void));
