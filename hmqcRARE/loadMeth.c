/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/RARE/loadMeth.c,v $
 *
 * Copyright (c) 2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: loadMeth.c,v 1.4 2005/08/30 15:54:19 sako Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: loadMeth.c,v 1.4 2005/08/30 15:54:19 sako Exp $(C) 2002 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


/*:=MPB=:=======================================================*
 *
 * Global Function: loadMeth
 *
 * Description: This procedure is automatically called in
 *	response to a method file for this method being read.
 *
 * Error History: 
 *
 * Interface:							*/

void loadMeth(const char *	className)
/*:=MPE=:=======================================================*/
{
  DB_MSG(( "-->loadMeth( %s )\n", className ));

  
 

  if ( (0 != className) && ( 0 == strcmp( className, "MethodClass" ))) 
  {
    /* converting the old protocol parameter to the new encoding parameter */ 
    if (ParxRelsParHasValue("PhaseEncodingStart"))
    {
     
      PVM_EncStart1 = PhaseEncodingStart;
    }
    initMeth();
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }
  
  DB_MSG(( "<--loadMeth( %s )\n", className ));
  
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
