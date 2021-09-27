# StreamDeckSocket
This listens on port 8080 of localhost. If the port is inconvenient, change it in "qhttpserver.h" as LISTEN_PORT 
do not change the LISTEN_ADDRESS unless you know what you mean to do.

The webserver accepts two paths:
 * http://localhost:8080/enumerate     -- returns all custom actions in a JSON object.
 * http://localhost:8080/action/<guid> -- invokes an action in DAZ identified by its GUID.
