;****************************************************************
;
; Copyright (c) 2003-2007
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; $Id: PrepModulesHead.mod,v 1.14.2.1 2007/07/02 13:05:36 mawi Exp $
;
; All Rights Reserved
;
; Declaration of pulseprogram parameters and subroutines for 
; preparation  modules
;
;****************************************************************

;**** Fat Suppression Module ****

#include "FatSupDef.mod"

;**** Flow Saturation Module ****

#include "FlowSatDef.mod" 

;**** Inversion Pulse Module ****
#include "InvPulseDef.mod"

;****CEST Magnetisation Transfer Module, cannot add MagTransDef.mod ****

#include "cest_MagTransDef.mod"

;**** Saturation Pulse Module ****

#include "SatPulseDef.mod"


;*** Saturation Slices Module ***

#include "SatSliceDef.mod"


;**** Black Blood Module ****
#include "BlBloodDef.mod"


;**** Trigger Module ****
define delay TrigD0 = {$PVM_TrigD0}
;**** Trigger Out Module ****
define delay TrigOutD0 = {$PVM_TrigOutD0}

;**** OuterVolumeSuppression **** Solvent Suppression Module ****

#include "WsOvsDef.mod"

;**** Solvent Suppression module and FOV Saturation module ******

#include "WsSatDef.mod"

;*********** selective inversion recovery **********

#include "SelSliceIrDef.mod"

;***********Tagging*****************

#include "TagDef.mod"

;*********** Noe *******************

#include "NoeDef.mod"



define loopcounter EvolLp0 = {$PVM_EvolLp0}
define list<delay> EvolD0 = {$PVM_EvolD0}


