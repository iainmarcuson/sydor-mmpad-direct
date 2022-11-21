TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

LIBRARY_IOC_Linux = mmpadDetector
LIBRARY_IOC_Darwin = mmpadDetector

USR_CXXFLAGS += -g -ggdb
USR_INCLUDES += -I$(TOP)/mmpadApp/cbfSrc -I/home/iainm/work/mmpad/mm-pad-client-cmdline/mm-pad-interface/include
#USR_LDFLAGS += -L/home/iainm/work/mmpad/mm-pad-client-cmdline/mm-pad-interface/lib/debug -lst_if_common -lst_if_client

LIB_SRCS += mmpadDetector.cpp

DBD += mmpadDetectorSupport.dbd

include $(ADCORE)/ADApp/commonLibraryMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

