stream = require("stream")

ReadTimeout = 4.0
OutTerminator = "\n\n"
InTerminator = "\n"

IP_PORT = stream.wrap("IP")

IP_PORT:write("GET / HTTP/1.0")
matched, HTTP_VER, ERROR_NO, MSG = IP_PORT:read("HTTP/%f %d %c")

if (matched) then
	print("HTTP Version: " .. HTTP_VER)
	print("Response Code: " .. ERROR_NO)
	print("Message: " .. MSG)
else
	print("Did not match")
end
