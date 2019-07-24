#include "streamDriver.h"
#include "luaEpics.h"
#include "epicsExport.h"
#include <stdio.h>

static int l_call(lua_State* state)
{	
	lua_getfield(state, 1, "name");
	const char* param_name = lua_tostring(state, -1);
	lua_pop(state, 1);
	
	lua_getglobal(state, "_protocols");
	lua_pushvalue(state, 2);
	lua_setfield(state, -2, param_name);
	
	return 0;
}

static int l_index(lua_State* state)
{
	static const luaL_Reg proto_set[] = {
		{"__call", l_call},
		{NULL, NULL}
	};
	
	const char* param_name = luaL_checkstring(state, 2);
	
	lua_newtable(state);
	lua_pushstring(state, param_name);
	lua_setfield(state, -2, "name");
	
	lua_newtable(state);
	luaL_setfuncs(state, proto_set, 0);
	lua_setmetatable(state, -2);
	
	return 1;
}

streamDriver::streamDriver(const char* port_name, const char* octet_port, const char* lua_filepath)
	:asynPortDriver( port_name, 1, 0, 0, 0, 1, 0, 0)
{
	static const luaL_Reg proto_get[] = {
		{"__index", l_index},
		{NULL, NULL}
	};
	
	this->state = luaCreateState();
	luaL_dostring(this->state, "asyn = require('asyn')");
	luaL_dostring(this->state, "stream = require('stream')");
	
	// port = stream(<octet_port>)
	lua_getglobal(this->state, "stream");
	lua_pushstring(this->state, octet_port);
	lua_call(this->state, 1, 1);
	lua_setglobal(this->state, "port");
	
	// _protocols will store all the definitions
	lua_newtable(state);
	lua_setglobal(this->state, "_protocols");

	luaL_newmetatable(this->state, "proto_get");
	luaL_setfuncs(this->state, proto_get, 0);
	lua_pop(this->state, 1);
	
	lua_getglobal(this->state, "_G");
	luaL_setmetatable(this->state, "proto_get");
	
	luaLoadScript(this->state, lua_filepath);
	
	// lua_getglobal(this->state, "stream");
	// lua_getfield(this->state, -1, "protocols");
	// lua_pushnil(this->state);
	
	// while(lua_next(state, -2))
	// {
		
	// }
}

// asynStatus streamDriver::writeInt32(asynUser* pasynuser, epicsInt32 value)
// {
	// char param_name[128] = { '\0' };
	
	// asynStatus status = this->getParamName(pasynuser->reason, &param_name);
	
	// return asynSuccess;
// }

streamDriver::~streamDriver()
{
	lua_close(this->state);
}

int lnewdriver(lua_State* state)
{
	lua_settop(state, 3);
	const char* port_name = luaL_checkstring(state, 1);
	const char* octet_port = luaL_checkstring(state, 2);
	const char* filepath = luaL_checkstring(state, 3);
	
	streamDriver* sd = new streamDriver(port_name, octet_port, filepath);
	
	return 0;
}
	
static void streamRegister(void)
{
	luaRegisterFunction("streamDriver", lnewdriver);
}

epicsExportRegistrar(streamRegister);
