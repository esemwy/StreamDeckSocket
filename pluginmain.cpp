#include <dzplugin.h>
#include <dzapp.h>

#include "version.h"
#include "streamdecksocketaction.h"

DZ_PLUGIN_DEFINITION("Stream Deck Plugin");

DZ_PLUGIN_AUTHOR("Esemwy");

DZ_PLUGIN_VERSION(PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD);

DZ_PLUGIN_DESCRIPTION(QString(
	"This plugin displays a log of messages within the Studio user interface. "
	"Thus, one can avoid the constant \"Help->Troubleshooting->View Log File...\" "
	"normally needed when tracking down problems."
));

DZ_PLUGIN_CLASS_GUID(StreamDeckSocketAction, 1e9277f2 - 21b4 - 11ea - a0af - 7085c22adcec);
