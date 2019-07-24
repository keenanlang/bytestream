#ifndef INC_STREAMDRIVER_H
#define INC_STREAMDRIVER_H

#include "asynPortDriver.h"
#include "luaEpics.h"

class streamDriver : public asynPortDriver
{
	public:
		streamDriver(const char* port_name, const char* octet_port, const char* lua_filepath);
		~streamDriver();
		
	private:
		lua_State* state;
};


#endif
