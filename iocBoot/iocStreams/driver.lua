OutTerminator = "\n"
InTerminator = "\n"

param.string "NAME" [[
	write "GET NAME"
	return read "NAME = %s"
]]
	
param.int32 "INTVAL" [[
	write "GET VALS"
	
	return read "VALS = %d, %s"
]]
