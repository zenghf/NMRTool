;****************************************************************
;
; Copyright (c) 2002-2007
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; All Rights Reserved
;
; MagTrans.mod : Magnetisation Transfer Module 
;
;****************************************************************
;
; PVM_MagTransPulse : sp27 (DPQQ[11].name ---> .Filename)
; 		      (DPQQ[11].power --> .Attenuation)
; PVM_MtP0          ---> PVM_MagTransPulse.Length)
;
; PVM_MtD1          : CONFIG_amplifier_enable
; PVM_MtD0          : PVM_RiseTime
;
; PVM_MtD3          : PVM_MagTransSpoilTime
; PVM_MtLp0         : PVM_MagTransPulsNumb
; PVM_MtD2          : PVM_MagTransInterDelay
;
;


subr cest_MagTrans()
