TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

LIBRARY_IOC += bytestream

DBD += bytestreamSupport.dbd

SRC_DIRS += $(TOP)/bytestreamApp/src/lpeg

bytestream_SRCS += lpcap.c
bytestream_SRCS += lpcode.c
bytestream_SRCS += lpprint.c
bytestream_SRCS += lptree.c
bytestream_SRCS += lpvm.c
bytestream_SRCS += llpeg.c

bytestream_LIBS += asyn
bytestream_LIBS += lua
bytestream_LIBS += $(EPICS_BASE_IOC_LIBS)


PROD_IOC_Linux = testStream

DBD += testStream.dbd
testStream_DBD += base.dbd
testStream_DBD += drvAsynIPPort.dbd
testStream_DBD += luaSupport.dbd
testStream_DBD += bytestreamSupport.dbd

testStream_LIBS += lua
testStream_LIBS += asyn
testStream_LIBS += bytestream
testStream_LIBS += $(EPICS_BASE_IOC_LIBS)

testStream_SRCS += testStream_registerRecordDeviceDriver.cpp
testStream_SRCS_DEFAULT += luaMain.cpp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
