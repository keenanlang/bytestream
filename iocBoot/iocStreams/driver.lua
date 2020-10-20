OutTerminator = "\n"
InTerminator = "\n"

param.string.read "NAME" [[
	write "GET NAME"
	return read "NAME = %s"
]]
	
param.int32.read "INTVAL" [[
	write "GET VALS"
	
	return read "VALS = %d, %s"
]]
