#include "epicsExport.h"
#include "luaEpics.h"

int luaopen_lpeg(lua_State* state);

static void liblpegRegister(void)    { luaRegisterLibrary("lpeg", luaopen_lpeg); }

epicsExportRegistrar(liblpegRegister);
