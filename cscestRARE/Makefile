# -------------------------------------------------------------
# /pvVersions/linux_intel/Pv51/prog/parx/src/cscestRARE/Makefile
# 
# Copyright (c) 2001-2004
# Bruker BioSpin MRI GmbH
# D-76275 Ettlingen, Germany
#
# All Rights Reserved
#
# To copy the directory and compile the copy it is only
# necessary to change DISKUNIT.
# -------------------------------------------------------------

# -------------------------------------------------------------
#  ParaVision linux templates
# -------------------------------------------------------------

EXTRA_STDLIBS =

# -------------------------------------------------------------
#  ParaVision templates
# -------------------------------------------------------------

LOCAL_DIRDEF = dir.def.2

XWINSHLIBDIR = $(PROGDIR)/shlib
XWINSHLIBDIROPT = -L$(XWINSHLIBDIR)

XWINSHARELIBEXT = $(SHARELIBEXT)

PVSHLIBDIR = $(PROGDIR)/pvshlib

MAKEDEPEND	= makedepend

RMD		= rm -rf

SHARCMD         = shar --quiet --text

SHARELIBBASEOPTS = -shared

# -------------------------------------------------------------
# Defaults for LAPACK / CLAPACK
# -------------------------------------------------------------

LAPACK_LIBRARIES = $(PVSHLIBDIR)/liblapack$(SHARELIBEXT) $(PVSHLIBDIR)/libblas$(SHARELIBEXT) $(SIMPLE_FORTRAN_LIBRARIES)

TOPSPIN_IMPORTLIBS = $(XWINSHLIBDIR)/editpar$(XWINSHARELIBEXT) $(XWINSHLIBDIR)/libacq$(XWINSHARELIBEXT)  $(XWINSHLIBDIR)/libPath$(XWINSHARELIBEXT) $(XWINSHLIBDIR)/libShapeIO$(XWINSHARELIBEXT) $(XWINSHLIBDIR)/libap$(XWINSHARELIBEXT) $(XWINSHLIBDIR)/libcb$(XWINSHARELIBEXT) $(XWINSHLIBDIR)/uni$(XWINSHARELIBEXT) $(XWINSHLIBDIR)/util$(XWINSHARELIBEXT)

TOPSPIN_PROGLIBS =

# -------------------------------------------------------------
# Define license libraries
# -------------------------------------------------------------

PVLICLIBS = $(LIBDIR)/libtlicl.a $(LIBDIR)/lm_new.o $(LIBDIR)/liblmgr.a $(LIBDIR)/libcrvs.a $(LIBDIR)/libsb.a

# -------------------------------------------------------------
# Define TCL definitions
# -------------------------------------------------------------

PVTCLDIR = $(USRLOCAL)/tcl/tcl8.4.11
PVTCLBINDIR = $(PVTCLDIR)/bin
PVTCLLIBDIR = $(PVTCLDIR)/lib

# -------------------------------------------------------------
# Perl program
# -------------------------------------------------------------

# -------------------------------------------------------------
# Regex library
# -------------------------------------------------------------

# -------------------------------------------------------------
# PCRE library
# -------------------------------------------------------------

PCRE_DIR =
PCRE_INCPATH = -I /usr/include/pcre
PCRE_LIBPATH =
PCRE_LIBRARIES = -lpcre
PCRE_COPYLIBS =

# -------------------------------------------------------------
# makeau compile definitions
# -------------------------------------------------------------

MAKEAU = XWINNMRHOME="$(DISKUNIT)" $(PERL) $(DISKUNIT)/exp/stan/nmr/au/makeau
MAKEAU_OPTIONS = -warnings $(MAKEAU_DEBUGFLAGS)
MAKEAU_DEBUGFLAGS = -optimize
AU_SRCDIR = $(PROGDIR)/au/tomo.exam
AU_BINDIR = $(PROGDIR)/au/tomo.bin

# -------------------------------------------------------------
# $Source: /bscl/CvsTree/bscl/gen/config/linux_intel.cf,v $
#
# Copyright (c) 2000
# BRUKER BIOSPIN GMBH
# D-76287 Rheinstetten, Germany
#
# All Rights Reserved
#
# $Id: linux_intel.cf,v 1.51.4.1 2007/03/28 14:00:56 uhei Exp $
# -------------------------------------------------------------

# -------------------------------------------------------------
# $Source: /bscl/CvsTree/bscl/gen/config/gcc.def,v $
#
# Copyright (c) 2000
# BRUKER BIOSPIN GMBH
# D-76287 Rheinstetten, Germany
#
# All Rights Reserved
#
# $Id: gcc.def,v 1.66.2.1.2.1 2006/12/12 10:12:25 roe Exp $
# -------------------------------------------------------------

# -------------------------------------------------------------
#
# $Source: /bscl/CvsTree/bscl/gen/config/CPlusPlus.tmpl,v $
#
# Copyright (c) 1997
# BRUKER BIOSPIN GMBH
# D-76287 Rheinstetten, Germany
#
# All Rights Reserved
#
# $Id: CPlusPlus.tmpl,v 1.19 2005/09/15 08:59:28 wem Exp $
#
# Defaults for various generic parameters for C++ programs.
# You may overwrite this defaults in either dir.def, site.def
# or linux_intel.cf
# -------------------------------------------------------------

CPLUSPLUS		= $(DISKUNIT)/gnu/bin/c++ -B$(DISKUNIT)/gnu/lib/gcc-lib/

STD_PLUSPLUS_LIBRARIES	= -ldl -lrt $(EXTRA_STDLIBS)
ALL_PLUSPLUS_LIBRARIES	= $(LIBRARIES) $(EXTRA_LIBRARIES) $(STD_PLUSPLUS_LIBRARIES)

STD_PLUSPLUS_LIBPATH 	=
ALL_PLUSPLUS_LIBPATH	= $(LIBPATH) $(STD_PLUSPLUS_LIBPATH)

STD_PLUSPLUS_INCPATH 	=
ALL_PLUSPLUS_INCPATH	= $(INCPATH) $(EXTRA_INCPATH) $(STD_PLUSPLUS_INCPATH)

STATICSTDCPLUSPLUS	= -static-libgcc -L $(COMPILER_ROOT)/staticlibstdcplusplus

SHAREDSTDCPLUSPLUS	= libgcc_s.so.1 libstdc++.so.6

STD_PLUSPLUS_DEFINES 	= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
ALL_PLUSPLUS_DEFINES	= $(OS_DEFINES) $(DEFINES) $(EXTRA_DEFINES) $(DLL_DEFINES) $(STD_PLUSPLUS_DEFINES)

CORBA_SOURCE_DEFINES	=

CPLUSPLUSDEBUGFLAGS	= -O2 -fPIC -DNDEBUG

CPLUSPLUSOPTIONS	= -Wall -W -Wpointer-arith -Wsign-compare -Wwrite-strings -Woverloaded-virtual -Wundef -Wdisabled-optimization -pedantic -Wno-long-long -fimplicit-templates
CPLUSPLUSLDOPTIONS	=  $(CPLUSPLUSDEBUGFLAGS)

CPLUSPLUSFLAGS		= $(CPLUSPLUSDEBUGFLAGS) $(CPLUSPLUSOPTIONS) $(ALL_PLUSPLUS_INCPATH) $(ALL_PLUSPLUS_DEFINES)

CPLUSPLUSLDFLAGS	= $(CPLUSPLUSLDOPTIONS) $(ALL_PLUSPLUS_LIBPATH)

PLUSPLUSDEPENDFLAGS 	= DependFlags

PLUSPLUSDEPEND		= Depend

PPSUFFIX		= .cc

# -------------------------------------------------------------
# C++ rules
# -------------------------------------------------------------

# -------------------------------------------------------------
# platform-specific configuration parameters - edit
# linux_intel.cf to change
# -------------------------------------------------------------

# -------------------------------------------------------------
# $Source: /bscl/CvsTree/bscl/gen/config/linux_intel.cf,v $
#
# Copyright (c) 2000
# BRUKER BIOSPIN GMBH
# D-76287 Rheinstetten, Germany
#
# All Rights Reserved
#
# $Id: linux_intel.cf,v 1.51.4.1 2007/03/28 14:00:56 uhei Exp $
# -------------------------------------------------------------

# -------------------------------------------------------------
# $Source: /bscl/CvsTree/bscl/gen/config/gcc.def,v $
#
# Copyright (c) 2000
# BRUKER BIOSPIN GMBH
# D-76287 Rheinstetten, Germany
#
# All Rights Reserved
#
# $Id: gcc.def,v 1.66.2.1.2.1 2006/12/12 10:12:25 roe Exp $
# -------------------------------------------------------------

# -------------------------------------------------------------
# Defaults for various generic parameters defined in
# "$(DISKUNIT)/gen/config/ParxExport.tmpl". You may overwrite this defaults in either
# dir.def, site.def or linux_intel.cf
# -------------------------------------------------------------

AR		= ar
ARCREATEFLAGS	= cr
ARUPDATEFLAGS	= ur

AWK		= awk

BASH		= bash

CAT		= cat

CC		= $(DISKUNIT)/gnu/bin/gcc -B$(DISKUNIT)/gnu/lib/gcc-lib/

CHMOD		= chmod

CMP		= cmp

CP		= cp

CP_R		= cp -r

CPASC		= cp

CPP		= /lib/cpp

DATE		= date

DIFF		= diff

ECHO		= echo -e

FIND		= find

GREP		= grep

GTAR		= tar

LD		= ld

LEX		= lex

LN		= ln

LS		= ls

M4		= m4

MAKE		= make

MAKEDEPEND	= makedepend

MKDIR		= mkdir -p

MV		= mv

PERL		= $(PERL_DIR)/bin/perl

RCP		= rcp

RM		= rm -f

RMDIR		= rm -rf

SED		= sed

SHARELIBCMD	=

SHELL		= sh

TAR		= tar

TOUCH		= touch

YACC		= yacc

NOOP		= true

STRIP		= strip

TOP		= .
CURRENT_DIR	= .

RM_CMD		= $(RM) *.CKP *.ln *.BAK *.bak *.o *.obj *.pch *.pdb *.ilk core errs ,* *~ *.a *.lib *.dll *.imp .emacs_* tags TAGS make.log MakeOut

APP_SUBSYSTEM	=

STD_LIBRARIES	= -ldl -lrt $(EXTRA_STDLIBS)
ALL_LIBRARIES	= $(LIBRARIES) $(EXTRA_LIBRARIES) $(STD_LIBRARIES)

STD_LIBPATH	=
ALL_LIBPATH	= $(LIBPATH) $(STD_LIBPATH)

STD_INCPATH	=
ALL_INCPATH	= $(INCPATH) $(EXTRA_INCPATH) $(STD_INCPATH)

STD_DEFINES	= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
OS_DEFINES	= -DLINUX_INTEL
DLL_DEFINES	=
ALL_DEFINES	= $(OS_DEFINES) $(DEFINES) $(EXTRA_DEFINES) $(DLL_DEFINES) $(STD_DEFINES)

CDEBUGFLAGS	= -O2 -fPIC -DNDEBUG

CCOPTIONS	= -Wall -W -Wpointer-arith -Wsign-compare -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Winline -Wundef -Wdisabled-optimization -pedantic -Wno-long-long -std=gnu99
CCLDOPTIONS	=
LDOPTIONS	=
DLLLDOPTIONS	=

CFLAGS		= $(CDEBUGFLAGS) $(CCOPTIONS) $(ALL_INCPATH) $(ALL_DEFINES)
CCLDFLAGS	= $(CDEBUGFLAGS) $(CCOPTIONS) $(CCLDOPTIONS) $(ALL_LIBPATH)

EXECEXT		=

OBJEXT		= .o

STATICLIBEXT	= .a

STATICCRTLIBEXT	= .a

DYNAMICLIBEXT	= .dll

SHARELIBEXT	= .so

CPROTO		= $(DISKUNIT)/prog/bin/cproto
CPROTOOPTIONS	= -E"$(CC) -E" -m -M_P_ -d -f2 -F"int\tmain ( a , b )"
CPROTO_DEFINES	= -DCPROTO -U__GNUC -U__GNUC__ -D__gnuc_va_list=int -D__builtin_va_list=int
CPROTOFLAGS	= $(CPROTOOPTIONS) $(CPROTO_DEFINES) $(ALL_DEFINES) $(ALL_INCPATH)

XMT_LIBRARIES	= /usr/local/libXmt-4.0.0/lib/libXmt.a
XMT_INCPATH	= -I/usr/local/libXmt-4.0.0/include

DHCP_INCPATH	= -I$(GLOBALHEADERDIR)/dhcp/isc/dhcpctl -I$(GLOBALHEADERDIR)/dhcp/isc/includes

XML_STATICLIBRARIES = /usr/local/libxml2-2.6.22/lib/libxml2.a -lpthread -lz
XML_STATICDEFINES =

SOAP_LIBRARIES	= -L/usr/local/gcc-3.4.6/axis-c-1.5.0/lib/axis -laxis_client -lhttp_channel -lhttp_transport -laxis_xerces /usr/local/gcc-3.4.6/xerces-2.7.0/lib/libxerces-c.so
SOAP_INCPATH	= -I/usr/local/gcc-3.4.6/axis-c-1.5.0/include

UPNP_LIBRARIES  = /usr/local/gcc-3.4.6/cyberlink-1.7.1/lib/libclink.a /usr/local/gcc-3.4.6/xerces-2.7.0/lib/libxerces-c.so
UPNP_INCPATH    = -I/usr/local/gcc-3.4.6/cyberlink-1.7.1/include

XERCES_LIBRARIES= /usr/local/gcc-3.4.6/xerces-2.7.0/lib/libxerces-c.so
XERCES_INCPATH	= -I/usr/local/gcc-3.4.6/xerces-2.7.0/include

MATH_LIBRARIES	= -lm

PTHREAD_LIBRARIES= -lpthread

FLEXLM_LIBRARIES= $(LIBDIR)/liblmgr.a -lpthread

# -------------------------------------------------------------
# $Source: /bscl/CvsTree/bscl/gen/config/Standard.tmpl,v $
#
# Copyright (c) 1995
# BRUKER BIOSPIN GMBH
# D-76287 Rheinstetten, Germany
#
# All Rights Reserved
#
# $Id: Standard.tmpl,v 1.23 2005/09/15 08:59:28 wem Exp $
# -------------------------------------------------------------

# -------------------------------------------------------------
# Standard build parameters
# -------------------------------------------------------------

DISKUNIT		= /opt/PV5.1

PROGDIR		= $(DISKUNIT)/prog

STANDALONEDIR	= $(DISKUNIT)/prog/bin

APPDEFDIR	= $(DISKUNIT)/prog/app-defaults

HINTSDIR	= $(DISKUNIT)/prog/hints

DOCUDIR		= $(DISKUNIT)/prog/docu

SHLIBDIR	= $(PROGDIR)/pvshlib

DLLDIR		= $(PROGDIR)/pvshlib

# For compatibilty to old Imakefiles - do not use !

LOCALHEADERDIR	= $(DISKUNIT)/prog/include
GLOBALHEADERDIR	= $(DISKUNIT)/prog/include

EXTRA_INCPATH	= -I$(LOCALHEADERDIR) -I$(GLOBALHEADERDIR)

PROTOHEADERDIR	= $(DISKUNIT)/prog/include/proto

CONFIG_INCLUDES	= -I. -I$(LOCALCONFIGDIR) -I$(GLOBALCONFIGDIR)

TMPDIR		= /tmp

PROTOHEAD	= $(DISKUNIT)/prog/include/proto/proto.head

DEPENDFLAGS	= -DMAKEDEPEND

# -------------------------------------------------------------
# Standard rule set
# -------------------------------------------------------------

DEPEND_START = \# DO NOT DELETE THIS LINE -- make depend depends on it

# -------------------------------------------------------------
# Start of Imakefile
# -------------------------------------------------------------

# ***************************************************************
#
# 	$Source: /pv/CvsTree/pv/gen/src/prg/methods/cscestRARE/Imakefile.distr,v $
#
# 	Copyright (c) 2001-2005
# 	Bruker BioSpin MRI GmbH
# 	D-76275 Ettlingen, Germany
#
# 	All Rights Reserved
#
# 	$Id: Imakefile.distr,v 1.5 2005/08/22 09:33:28 sako Exp $
#
# ***************************************************************/

PARCOMP_OPTS     = -DInParxOvl
PARCOMP_FLAGS	 = $(ALL_INCPATH) $(DEFINES) $(PARCOMP_OPTS) $(PARCOMP_DEBUG)
PARCOMP_DEBUG    =
PARCOMP	         = $(DISKUNIT)/prog/bin/scripts/methcomp

PARX_OVT_FLAGS	 =
PARX_OVE_FLAGS	 = -P $(PULPROG) $(PULPROG_INC)

PARX_OVL_LIBPATH = -L$(DISKUNIT)/gnu/lib -L$(SHLIBDIR) -L$(XWINSHLIBDIR)
PARX_OVL_LIBS	 = $(LIBRARIES) $(STD_LIBRARIES)
PARX_OVL_CPP     =
PARX_OVL_LD      = $(DISKUNIT)/gnu/bin/c++ -B$(DISKUNIT)/gnu/lib/gcc-lib/
PARX_OVL_LDOPTS  = -shared -Wl,-no-undefined $(CCLDOPTIONS)
PARX_OVL_EXT     = .so
PARX_OVL_AR      = $(AR)
PARX_OVL_ARFLAGS =
PARX_TK_EXT      = .so
PARX_DOC_DIR     = $(LOCALDOCUROOT)/prg
PARX_DOC_EXT     = _doc
PARX_DXX_EXT     = .dxx
PARX_DPP_EXT     = .dpp
PARX_DXX         = $(LOCALBINDIR)/pdoc++
PARX_DPP         = $(PERL) $(LOCALBINDIR)/pdocmerge

PARX_PP_STD_INCL = -p $(EXP_LISTS_DIR)/pp -p $(EXP_LISTS_DIR)/pp.dtomo -p $(EXP_LISTS_DIR)/pp.dexam

PARX_OVERLAY_DIR = $(PROGDIR)/parx/bin
PARX_TOOLKIT_DIR = $(DLLDIR)
PARX_METHODS_DIR = $(PROGDIR)/parx/methods
PARX_SERVICE_METHODS_DIR = $(PROGDIR)/service/methods
METHODS_DIR	 = /home/nmrsu/methods/bin

EXP_LISTS_DIR    = $(DISKUNIT)/exp/stan/nmr/lists

OVERLAY		= cscestRARE
PULPROG		= none
PULPROG_INC	=

SRCDIR		= /home/nmrsu/methods/src/cscestRARE

INCPATH		= -I$(PROGDIR)/include -I.

RELOBJS		=	\
			initMeth$(OBJEXT) \
			loadMeth$(OBJEXT) \
			parsRelations$(OBJEXT) \
			BaseLevelRelations$(OBJEXT) \
			RecoRelations$(OBJEXT) 

OBJS		= 		  $(OVERLAY)$(OBJEXT) 		  $(RELOBJS)

SRCS		= 		  `for i in $(OBJS) ; do 			echo $(SRCDIR)/\`basename $$i $(OBJEXT)\`.c ; 		   done`

RELSRCS		= 		  `for i in $(RELOBJS) ; do 			echo $(SRCDIR)/\`basename $$i $(OBJEXT)\`.c ; 		   done`

LIBRARIES	= 		  -lPvUtilTools                   -lPvCfgTools 		  -lPvAcqTools 		  -lPvPvmTools 		  -lPvMrTools 		  -lPvGeoTools 		  -lPvSeqTools 		  -lPvPvmTools                   -lPvOvlTools 		  -lParxRels 		  -lParxBase 		  -lm

IMPORTLIBS	= 		  $(SHLIBDIR)/libParxRels$(SHARELIBEXT)

all::	$(OVERLAY)$(PARX_OVL_EXT)

install:: $(OVERLAY)$(PARX_OVL_EXT) test
	if [ ! -d $(METHODS_DIR) ]; then $(MKDIR) $(METHODS_DIR) ; fi

install:: test
	$(RM) $(METHODS_DIR)/`basename $(OVERLAY)$(PARX_OVL_EXT)`
	$(CP) $(OVERLAY)$(PARX_OVL_EXT) $(METHODS_DIR)/`basename $(OVERLAY)$(PARX_OVL_EXT)`

clean::
	$(RM) $(OVERLAY)$(PARX_OVL_EXT)

$(OVERLAY)$(PARX_OVL_EXT): $(OBJS)
	$(RM) $(OVERLAY)$(PARX_OVL_EXT)
	$(PARX_OVL_LD) $(PARX_OVL_LDOPTS) $(OBJS) \
	$(PARX_OVL_LIBPATH) $(PARX_OVL_LIBS) -o $(OVERLAY)$(PARX_OVL_EXT)

depend::
	$(RM) Makefile.bak
	$(MV) Makefile Makefile.bak
	@$(SED) '/^$(DEPEND_START)/,$$d' < Makefile.bak > Makefile
	@$(ECHO) "$(DEPEND_START)" >> Makefile
	$(CPLUSPLUS) -M $(CPLUSPLUSFLAGS) $(DEPENDFLAGS) $(SRCS) >> Makefile

$(RELOBJS): $(OVERLAY)$(OBJEXT)
	$(PARCOMP) $(SRCDIR)/$(OVERLAY).c $(PARCOMP_FLAGS) -r $(SRCDIR)/$(@:$(OBJEXT)=.c)

$(OVERLAY)$(OBJEXT):
	$(PARCOMP) $(SRCDIR)/$(OVERLAY).c   $(PARX_OVE_FLAGS) $(PARCOMP_FLAGS)

cproto::
	-if [ ! -r relProtos_p.h ]; then			\
	$(TOUCH) relProtos_p.h;			\
	fi
	$(CAT) < $(PROTOHEAD) > relProtos_p.h.new
	$(CPROTO) $(CPROTOFLAGS) $(RELSRCS) >> relProtos_p.h.new
	$(CMP) -s relProtos_p.h relProtos_p.h.new || \
	{ \
	$(ECHO) "install new prototype file"; \
	$(RM) relProtos_p.h; \
	$(MV) relProtos_p.h.new relProtos_p.h; \
	}
	$(RM) relProtos_p.h.new;

# -------------------------------------------------------------
# Common rules for all Makefiles - do not edit
# -------------------------------------------------------------

emptyrule::

clean::
	$(RM_CMD)

Makefile::
	-@if [ -f Makefile ]; then set -x; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -f$(SRCDIR)/Imakefile -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

# -------------------------------------------------------------
# Empty rules for directories that do not have SUBDIRS
# -------------------------------------------------------------

depend::

Makefiles::

all::

test::

install::

cproto::

dll::

idl::

docu::

# -------------------------------------------------------------
# Dependencies generated by makedepend
# -------------------------------------------------------------

# DO NOT DELETE THIS LINE -- make depend depends on it
cscestRARE.o: /home/nmrsu/methods/src/cscestRARE/cscestRARE.c \
  /opt/PV5.1/prog/include/bruktyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/gradient_ramp_typ.h \
  /opt/PV5.1/prog/include/acqumtyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/recotyp.h \
  /opt/PV5.1/prog/include/Reco/RecoStageTyp.h \
  /opt/PV5.1/prog/include/subjtyp.h \
  /opt/PV5.1/prog/include/generated/ParxDefs.h \
  /opt/PV5.1/prog/include/ta_config.h \
  /opt/PV5.1/prog/include/methodTypes.h \
  /opt/PV5.1/prog/include/lib/PvSysinfoClient_state.h \
  /opt/PV5.1/prog/include/globalMethTypes.h \
  /opt/PV5.1/prog/include/adjManagerTypes.h \
  /opt/PV5.1/prog/include/tomo/StoreTime.h \
  /opt/PV5.1/prog/include/adjManagerDefs.h \
  /opt/PV5.1/prog/include/seqApiTypes.h \
  /opt/PV5.1/prog/include/epiTypes.h \
  /opt/PV5.1/prog/include/SpiralTypes.h \
  /opt/PV5.1/prog/include/proto/acq_extern.h \
  /opt/PV5.1/prog/include/proto/subj_extern.h \
  /opt/PV5.1/prog/include/proto/pvm_extern.h \
  /opt/PV5.1/prog/include/methodFormat.h \
  /home/nmrsu/methods/src/cscestRARE/parsTypes.h \
  /home/nmrsu/methods/src/cscestRARE/parsDefinition.h \
  /home/nmrsu/methods/src/cscestRARE/callbackDefs.h \
  /opt/PV5.1/prog/include/methodClassDefs.h \
  /opt/PV5.1/prog/include/digitizerClassDefs.h \
  /opt/PV5.1/prog/include/epiClassDefs.h \
  /opt/PV5.1/prog/include/SpiralCls.h \
  /opt/PV5.1/prog/include/SpirTrajCls.h \
  /opt/PV5.1/prog/include/diffusionClassDefs.h \
  /opt/PV5.1/prog/include/seqApiClassDefs.h \
  /opt/PV5.1/prog/include/modulesClassDefs.h \
  /home/nmrsu/methods/src/cscestRARE/parsLayout.h
initMeth.o: /home/nmrsu/methods/src/cscestRARE/initMeth.c \
  /home/nmrsu/methods/src/cscestRARE/method.h \
  /opt/PV5.1/prog/include/machine.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/bits/mathinline.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/limits.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /opt/PV5.1/prog/include/brukdef.h /opt/PV5.1/prog/include/bruktyp.h \
  /opt/PV5.1/prog/include/Parx/Parx.h \
  /opt/PV5.1/prog/include/Parx/libParx.h \
  /opt/PV5.1/prog/include/ta_config.h /opt/PV5.1/prog/include/debugdef.h \
  /opt/PV5.1/prog/include/lib/libPvPath.h \
  /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/gradient_ramp_typ.h \
  /opt/PV5.1/prog/include/acqumtyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/recotyp.h \
  /opt/PV5.1/prog/include/Reco/RecoStageTyp.h \
  /opt/PV5.1/prog/include/subjtyp.h \
  /opt/PV5.1/prog/include/generated/ParxDefs.h \
  /opt/PV5.1/prog/include/methodTypes.h \
  /opt/PV5.1/prog/include/lib/PvSysinfoClient_state.h \
  /opt/PV5.1/prog/include/globalMethTypes.h \
  /opt/PV5.1/prog/include/adjManagerTypes.h \
  /opt/PV5.1/prog/include/tomo/StoreTime.h \
  /opt/PV5.1/prog/include/adjManagerDefs.h \
  /opt/PV5.1/prog/include/seqApiTypes.h \
  /opt/PV5.1/prog/include/epiTypes.h \
  /opt/PV5.1/prog/include/SpiralTypes.h \
  /opt/PV5.1/prog/include/PVMTools.h \
  /opt/PV5.1/prog/include/PvUtilTools.h \
  /opt/PV5.1/prog/include/proto/PvUtilTools_p.h \
  /opt/PV5.1/prog/include/PvAcqTools.h \
  /opt/PV5.1/prog/include/proto/PvAcqTools_p.h \
  /opt/PV5.1/prog/include/PvCfgTools.h \
  /opt/PV5.1/prog/include/lib/libPvCfg.h \
  /opt/PV5.1/prog/include/acqutyp.h /opt/PV5.1/prog/include/acqumtyp.h \
  /opt/PV5.1/prog/include/proto/PvCfgTools_p.h \
  /opt/PV5.1/prog/include/PvGeoTools.h \
  /opt/PV5.1/prog/include/proto/PvGeoTools_p.h \
  /opt/PV5.1/prog/include/PvMrTools.h \
  /opt/PV5.1/prog/include/proto/PvMrTools_p.h \
  /opt/PV5.1/prog/include/PvPvmTools.h \
  /opt/PV5.1/prog/include/proto/PvPvmTools_p.h \
  /opt/PV5.1/prog/include/PvSeqTools.h \
  /opt/PV5.1/prog/include/proto/PvSeqTools_p.h \
  /opt/PV5.1/prog/include/ovl_toolbox/RecoTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/VisuTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/Utils.h \
  /home/nmrsu/methods/src/cscestRARE/parsTypes.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos_p.h
loadMeth.o: /home/nmrsu/methods/src/cscestRARE/loadMeth.c \
  /home/nmrsu/methods/src/cscestRARE/method.h \
  /opt/PV5.1/prog/include/machine.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/bits/mathinline.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/limits.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /opt/PV5.1/prog/include/brukdef.h /opt/PV5.1/prog/include/bruktyp.h \
  /opt/PV5.1/prog/include/Parx/Parx.h \
  /opt/PV5.1/prog/include/Parx/libParx.h \
  /opt/PV5.1/prog/include/ta_config.h /opt/PV5.1/prog/include/debugdef.h \
  /opt/PV5.1/prog/include/lib/libPvPath.h \
  /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/gradient_ramp_typ.h \
  /opt/PV5.1/prog/include/acqumtyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/recotyp.h \
  /opt/PV5.1/prog/include/Reco/RecoStageTyp.h \
  /opt/PV5.1/prog/include/subjtyp.h \
  /opt/PV5.1/prog/include/generated/ParxDefs.h \
  /opt/PV5.1/prog/include/methodTypes.h \
  /opt/PV5.1/prog/include/lib/PvSysinfoClient_state.h \
  /opt/PV5.1/prog/include/globalMethTypes.h \
  /opt/PV5.1/prog/include/adjManagerTypes.h \
  /opt/PV5.1/prog/include/tomo/StoreTime.h \
  /opt/PV5.1/prog/include/adjManagerDefs.h \
  /opt/PV5.1/prog/include/seqApiTypes.h \
  /opt/PV5.1/prog/include/epiTypes.h \
  /opt/PV5.1/prog/include/SpiralTypes.h \
  /opt/PV5.1/prog/include/PVMTools.h \
  /opt/PV5.1/prog/include/PvUtilTools.h \
  /opt/PV5.1/prog/include/proto/PvUtilTools_p.h \
  /opt/PV5.1/prog/include/PvAcqTools.h \
  /opt/PV5.1/prog/include/proto/PvAcqTools_p.h \
  /opt/PV5.1/prog/include/PvCfgTools.h \
  /opt/PV5.1/prog/include/lib/libPvCfg.h \
  /opt/PV5.1/prog/include/acqutyp.h /opt/PV5.1/prog/include/acqumtyp.h \
  /opt/PV5.1/prog/include/proto/PvCfgTools_p.h \
  /opt/PV5.1/prog/include/PvGeoTools.h \
  /opt/PV5.1/prog/include/proto/PvGeoTools_p.h \
  /opt/PV5.1/prog/include/PvMrTools.h \
  /opt/PV5.1/prog/include/proto/PvMrTools_p.h \
  /opt/PV5.1/prog/include/PvPvmTools.h \
  /opt/PV5.1/prog/include/proto/PvPvmTools_p.h \
  /opt/PV5.1/prog/include/PvSeqTools.h \
  /opt/PV5.1/prog/include/proto/PvSeqTools_p.h \
  /opt/PV5.1/prog/include/ovl_toolbox/RecoTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/VisuTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/Utils.h \
  /home/nmrsu/methods/src/cscestRARE/parsTypes.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos_p.h
parsRelations.o: /home/nmrsu/methods/src/cscestRARE/parsRelations.c \
  /home/nmrsu/methods/src/cscestRARE/method.h \
  /opt/PV5.1/prog/include/machine.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/bits/mathinline.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/limits.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /opt/PV5.1/prog/include/brukdef.h /opt/PV5.1/prog/include/bruktyp.h \
  /opt/PV5.1/prog/include/Parx/Parx.h \
  /opt/PV5.1/prog/include/Parx/libParx.h \
  /opt/PV5.1/prog/include/ta_config.h /opt/PV5.1/prog/include/debugdef.h \
  /opt/PV5.1/prog/include/lib/libPvPath.h \
  /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/gradient_ramp_typ.h \
  /opt/PV5.1/prog/include/acqumtyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/recotyp.h \
  /opt/PV5.1/prog/include/Reco/RecoStageTyp.h \
  /opt/PV5.1/prog/include/subjtyp.h \
  /opt/PV5.1/prog/include/generated/ParxDefs.h \
  /opt/PV5.1/prog/include/methodTypes.h \
  /opt/PV5.1/prog/include/lib/PvSysinfoClient_state.h \
  /opt/PV5.1/prog/include/globalMethTypes.h \
  /opt/PV5.1/prog/include/adjManagerTypes.h \
  /opt/PV5.1/prog/include/tomo/StoreTime.h \
  /opt/PV5.1/prog/include/adjManagerDefs.h \
  /opt/PV5.1/prog/include/seqApiTypes.h \
  /opt/PV5.1/prog/include/epiTypes.h \
  /opt/PV5.1/prog/include/SpiralTypes.h \
  /opt/PV5.1/prog/include/PVMTools.h \
  /opt/PV5.1/prog/include/PvUtilTools.h \
  /opt/PV5.1/prog/include/proto/PvUtilTools_p.h \
  /opt/PV5.1/prog/include/PvAcqTools.h \
  /opt/PV5.1/prog/include/proto/PvAcqTools_p.h \
  /opt/PV5.1/prog/include/PvCfgTools.h \
  /opt/PV5.1/prog/include/lib/libPvCfg.h \
  /opt/PV5.1/prog/include/acqutyp.h /opt/PV5.1/prog/include/acqumtyp.h \
  /opt/PV5.1/prog/include/proto/PvCfgTools_p.h \
  /opt/PV5.1/prog/include/PvGeoTools.h \
  /opt/PV5.1/prog/include/proto/PvGeoTools_p.h \
  /opt/PV5.1/prog/include/PvMrTools.h \
  /opt/PV5.1/prog/include/proto/PvMrTools_p.h \
  /opt/PV5.1/prog/include/PvPvmTools.h \
  /opt/PV5.1/prog/include/proto/PvPvmTools_p.h \
  /opt/PV5.1/prog/include/PvSeqTools.h \
  /opt/PV5.1/prog/include/proto/PvSeqTools_p.h \
  /opt/PV5.1/prog/include/ovl_toolbox/RecoTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/VisuTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/Utils.h \
  /home/nmrsu/methods/src/cscestRARE/parsTypes.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos_p.h
BaseLevelRelations.o:  \
 /home/nmrsu/methods/src/cscestRARE/BaseLevelRelations.c \
  /home/nmrsu/methods/src/cscestRARE/method.h \
  /opt/PV5.1/prog/include/machine.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/bits/mathinline.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/limits.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /opt/PV5.1/prog/include/brukdef.h /opt/PV5.1/prog/include/bruktyp.h \
  /opt/PV5.1/prog/include/Parx/Parx.h \
  /opt/PV5.1/prog/include/Parx/libParx.h \
  /opt/PV5.1/prog/include/ta_config.h /opt/PV5.1/prog/include/debugdef.h \
  /opt/PV5.1/prog/include/lib/libPvPath.h \
  /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/gradient_ramp_typ.h \
  /opt/PV5.1/prog/include/acqumtyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/recotyp.h \
  /opt/PV5.1/prog/include/Reco/RecoStageTyp.h \
  /opt/PV5.1/prog/include/subjtyp.h \
  /opt/PV5.1/prog/include/generated/ParxDefs.h \
  /opt/PV5.1/prog/include/methodTypes.h \
  /opt/PV5.1/prog/include/lib/PvSysinfoClient_state.h \
  /opt/PV5.1/prog/include/globalMethTypes.h \
  /opt/PV5.1/prog/include/adjManagerTypes.h \
  /opt/PV5.1/prog/include/tomo/StoreTime.h \
  /opt/PV5.1/prog/include/adjManagerDefs.h \
  /opt/PV5.1/prog/include/seqApiTypes.h \
  /opt/PV5.1/prog/include/epiTypes.h \
  /opt/PV5.1/prog/include/SpiralTypes.h \
  /opt/PV5.1/prog/include/PVMTools.h \
  /opt/PV5.1/prog/include/PvUtilTools.h \
  /opt/PV5.1/prog/include/proto/PvUtilTools_p.h \
  /opt/PV5.1/prog/include/PvAcqTools.h \
  /opt/PV5.1/prog/include/proto/PvAcqTools_p.h \
  /opt/PV5.1/prog/include/PvCfgTools.h \
  /opt/PV5.1/prog/include/lib/libPvCfg.h \
  /opt/PV5.1/prog/include/acqutyp.h /opt/PV5.1/prog/include/acqumtyp.h \
  /opt/PV5.1/prog/include/proto/PvCfgTools_p.h \
  /opt/PV5.1/prog/include/PvGeoTools.h \
  /opt/PV5.1/prog/include/proto/PvGeoTools_p.h \
  /opt/PV5.1/prog/include/PvMrTools.h \
  /opt/PV5.1/prog/include/proto/PvMrTools_p.h \
  /opt/PV5.1/prog/include/PvPvmTools.h \
  /opt/PV5.1/prog/include/proto/PvPvmTools_p.h \
  /opt/PV5.1/prog/include/PvSeqTools.h \
  /opt/PV5.1/prog/include/proto/PvSeqTools_p.h \
  /opt/PV5.1/prog/include/ovl_toolbox/RecoTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/VisuTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/Utils.h \
  /home/nmrsu/methods/src/cscestRARE/parsTypes.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos_p.h
RecoRelations.o: /home/nmrsu/methods/src/cscestRARE/RecoRelations.c \
  /home/nmrsu/methods/src/cscestRARE/method.h \
  /opt/PV5.1/prog/include/machine.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h /usr/include/stdlib.h \
  /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/xlocale.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
  /usr/include/bits/inf.h /usr/include/bits/nan.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/bits/mathinline.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/limits.h \
  /opt/PV5.1/gnu/bin/../lib/gcc/i686-pc-linux-gnu/3.4.6/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/xopen_lim.h \
  /opt/PV5.1/prog/include/brukdef.h /opt/PV5.1/prog/include/bruktyp.h \
  /opt/PV5.1/prog/include/Parx/Parx.h \
  /opt/PV5.1/prog/include/Parx/libParx.h \
  /opt/PV5.1/prog/include/ta_config.h /opt/PV5.1/prog/include/debugdef.h \
  /opt/PV5.1/prog/include/lib/libPvPath.h \
  /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/gradient_ramp_typ.h \
  /opt/PV5.1/prog/include/acqumtyp.h /opt/PV5.1/prog/include/acqutyp.h \
  /opt/PV5.1/prog/include/recotyp.h \
  /opt/PV5.1/prog/include/Reco/RecoStageTyp.h \
  /opt/PV5.1/prog/include/subjtyp.h \
  /opt/PV5.1/prog/include/generated/ParxDefs.h \
  /opt/PV5.1/prog/include/methodTypes.h \
  /opt/PV5.1/prog/include/lib/PvSysinfoClient_state.h \
  /opt/PV5.1/prog/include/globalMethTypes.h \
  /opt/PV5.1/prog/include/adjManagerTypes.h \
  /opt/PV5.1/prog/include/tomo/StoreTime.h \
  /opt/PV5.1/prog/include/adjManagerDefs.h \
  /opt/PV5.1/prog/include/seqApiTypes.h \
  /opt/PV5.1/prog/include/epiTypes.h \
  /opt/PV5.1/prog/include/SpiralTypes.h \
  /opt/PV5.1/prog/include/PVMTools.h \
  /opt/PV5.1/prog/include/PvUtilTools.h \
  /opt/PV5.1/prog/include/proto/PvUtilTools_p.h \
  /opt/PV5.1/prog/include/PvAcqTools.h \
  /opt/PV5.1/prog/include/proto/PvAcqTools_p.h \
  /opt/PV5.1/prog/include/PvCfgTools.h \
  /opt/PV5.1/prog/include/lib/libPvCfg.h \
  /opt/PV5.1/prog/include/acqutyp.h /opt/PV5.1/prog/include/acqumtyp.h \
  /opt/PV5.1/prog/include/proto/PvCfgTools_p.h \
  /opt/PV5.1/prog/include/PvGeoTools.h \
  /opt/PV5.1/prog/include/proto/PvGeoTools_p.h \
  /opt/PV5.1/prog/include/PvMrTools.h \
  /opt/PV5.1/prog/include/proto/PvMrTools_p.h \
  /opt/PV5.1/prog/include/PvPvmTools.h \
  /opt/PV5.1/prog/include/proto/PvPvmTools_p.h \
  /opt/PV5.1/prog/include/PvSeqTools.h \
  /opt/PV5.1/prog/include/proto/PvSeqTools_p.h \
  /opt/PV5.1/prog/include/ovl_toolbox/RecoTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/VisuTools.h \
  /opt/PV5.1/prog/include/ovl_toolbox/Utils.h \
  /home/nmrsu/methods/src/cscestRARE/parsTypes.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos.h \
  /home/nmrsu/methods/src/cscestRARE/relProtos_p.h
