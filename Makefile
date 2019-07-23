#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS += configure streamsApp
luaApp_DEPEND_DIRS = configure
include $(TOP)/configure/RULES_TOP
