# StreamDeckSocket
This listens on port 8080 of localhost by default, but can be changed in DAZ Studio under Edit -> Stream Deck.

The webserver accepts two paths:
 * http://localhost:8080/enumerate     -- returns all custom actions in a JSON object.
 * http://localhost:8080/action/<guid> -- invokes an action in DAZ identified by its GUID.
