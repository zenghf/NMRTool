;****************************************************************
; $Source: /pv/CvsTree/pv/gen/src/prg/methManag/PPG/MagTransDef.mod,v $
;
; Copyright (c) 2005-2007
; Bruker BioSpin MRI GmbH
; D-76275 Ettlingen, Germany
;
; All Rights Reserved
;
; MagTransDef.mod: Declaration of subroutine for Magnetization 
;                  transfer 
; call
; subr MagTrans()
;
; $Id: MagTransDef.mod,v 1.1.2.3 2008/06/05 17:10:28 mawi Exp $
;****************************************************************
;**** Magnetisation Transfer Module ****



if(PVM_MagTransOnOff)
{
define list <frequency> o3list = {$ACQ_O3_list}
define list <delay> M0list = {$M0Flag}

define delay MtD0 = {$PVM_MtD0}
define delay MtD1 = {$PVM_MtD1}
define delay MtD2 = {$PVM_MtD2}
define delay MtD3 = {$PVM_MtD3}



define pulse MtP0 = {$PVM_MtP0}

define loopcounter MtLp0 = {$PVM_MtLp0}
define delay MTtime 
"MTtime=MtD1+(MtD2+MtP0)*MtLp0"

}

subroutine cest_MagTrans()
{

  if(PVM_MagTransOnOff)
  {

      5u
     if "M0list <0.5" goto MTpart
     MTtime
     goto skipMT
  MTpart,   MtD1  gatepulse 1
  mtc, 2u o3list:f1

      MtP0:sp27
      MtD2
      lo to mtc times MtLp0

  skipMT,    MtD3  grad{(0)|(0)|PVM_MagTransSpoiler(100,,no_scale)}
      MtD0       	groff
      100u	
  }
}
