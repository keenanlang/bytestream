#include <string.h>

#include "streamDriver.h"
#include "luaEpics.h"
#include "epicsExport.h"
#include <stdio.h>

static int l_call(lua_State* state)
{
	lua_getfield(state, 1, "name");
	const char* param_name = lua_tostring(state, -1);
	lua_pop(state, 1);

	const char* data = luaL_checkstring(state, 2);
	
	lua_getglobal(state, "_protocols");
	lua_pushvalue(state, 1);
	lua_pushstring(state, data);
	lua_setfield(state, -2, "func");
	lua_setfield(state, -2, param_name);
	lua_pop(state, 1);
	
	return 0;
}

static int l_addname(lua_State* state)
{
	static const luaL_Reg proto_set[] = {
		{"__call", l_call},
		{NULL, NULL}
	};
	
	const char* param_name = luaL_checkstring(state, 2);
	
	lua_newtable(state);
	lua_pushstring(state, param_name);
	lua_setfield(state, -2, "name");
	
	lua_getfield(state, 1, "type");
	lua_setfield(state, -2, "type");
	
	luaL_newmetatable(state, "proto_set");
	luaL_setfuncs(state, proto_set, 0);
	lua_pop(state, 1);
	
	luaL_setmetatable(state, "proto_set");
	
	return 1;
}

static int l_index(lua_State* state)
{
	const char* param_type = luaL_checkstring(state, 2);
	
	lua_newtable(state);
	
	if      (strcasecmp(param_type, "int32") == 0)         { lua_pushinteger(state, 1); }
	else if (strcasecmp(param_type, "uint32digital") == 0) { lua_pushinteger(state, 2); }
	else if (strcasecmp(param_type, "float64") == 0)       { lua_pushinteger(state, 3); }
	else if (strcasecmp(param_type, "string") == 0)        { lua_pushinteger(state, 4); }
	else                                                   { lua_pushinteger(state, 0); }
	
	lua_setfield(state, -2, "type");
	
	static const luaL_Reg param_call[] = {
		{"__call", l_addname},
		{NULL, NULL}
	};
	
	luaL_newmetatable(state, "param_call");
	luaL_setfuncs(state, param_call, 0);
	lua_pop(state, 1);
	
	luaL_setmetatable(state, "param_call");
	
	return 1;
}

streamDriver::streamDriver(const char* port_name, const char* octet_port, const char* lua_filepath)
	:asynPortDriver( port_name, 1, 0, 0, 0, 1, 0, 0)
{
	static const luaL_Reg param_get[] = {
		{"__index", l_index},
		{NULL, NULL}
	};
	
	this->state = luaCreateState();
	luaL_dostring(this->state, "asyn = require('asyn')");
	luaL_dostring(this->state, "stream = require('stream')");
	
	luaL_newmetatable(this->state, "param_get");
	luaL_setfuncs(this->state, param_get, 0);
	lua_pop(this->state, 1);
	
	lua_newtable(this->state);
	luaL_setmetatable(this->state, "param_get");
	lua_setglobal(this->state, "param");
	
	// port = stream(<octet_port>)
	lua_getglobal(this->state, "stream");
	lua_pushstring(this->state, octet_port);
	lua_call(this->state, 1, 1);
	lua_setglobal(this->state, "port");
	
	// _protocols will store all the definitions
	lua_newtable(state);
	lua_setglobal(this->state, "_protocols");
	
	int status = luaLoadScript(this->state, lua_filepath);
	
	if (status) { const char* msg = lua_tostring(state, -1); printf("%s\n", msg);}
	
	
	lua_getglobal(this->state, "_protocols");
	lua_pushnil(this->state);
	
	int unused;
	
	while(lua_next(state, -2))
	{
		lua_getfield(this->state, -1, "name");
		const char* param_name = lua_tostring(this->state, -1);
		lua_pop(this->state, 1);
		
		lua_getfield(this->state, -1, "type");
		int param_type = luaL_optinteger(this->state, -1, 1);
		lua_pop(this->state, 1);
		
		this->createParam(param_name, (asynParamType) param_type, &unused);
		
		lua_pop(this->state, 1);
	}
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
