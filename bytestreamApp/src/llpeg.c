#include "epicsExport.h"
#include "luaEpics.h"

int luaopen_lpeg(lua_State* state);

static void llpegRegister(void)    { luaRegisterLibrary("lpeg", luaopen_lpeg); }

epicsExportRegistrar(llpegRegister);
