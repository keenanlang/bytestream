epicsEnvSet("LUA_SCRIPT_PATH", ".:./scripts")

dbLoadDatabase("../../dbd/testStream.dbd")
testStream_registerRecordDeviceDriver(pdbbase)

drvAsynIPPortConfigure("IP", "www.google.com:80", 0, 0, 0)

---------------
iocInit()
---------------

--luaSpawn("./test.lua", "PORT='IP'")

streamDriver("TEST", "IP", "driver.lua")
