OutTerminator = "\n"
InTerminator = "\n"

NAME [[
	write "GET NAME"
	return read "NAME = %s"
]]
	
INTVAL [[
	write "GET VALS"
	
	return read "VALS = %d, %s"
]]

print(_protocols.NAME)
