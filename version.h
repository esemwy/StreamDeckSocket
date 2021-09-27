/**********************************************************************
Copyright (C) 2002-2012 DAZ 3D, Inc. All Rights Reserved.

This file is part of the DAZ Studio SDK.

This file may be used only in accordance with the DAZ Studio SDK
license provided with the DAZ Studio SDK.

The contents of this file may not be disclosed to third parties,
copied or duplicated in any form, in whole or in part, without the
prior written permission of DAZ 3D, Inc, except as explicitly
allowed in the DAZ Studio SDK license.

See http://www.daz3d.com to contact DAZ 3D or for more
information about the DAZ Studio SDK.
**********************************************************************/

#ifndef VERSION_H
#define VERSION_H

#include <dzversion.h>

// Version number for AFirstPlugin
#define PLUGIN_MAJOR	1
#define PLUGIN_MINOR	0
#define PLUGIN_REV		0
#define PLUGIN_BUILD	1

#define PLUGIN_VERSION	DZ_MAKE_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD )

#endif // VERSION_H
