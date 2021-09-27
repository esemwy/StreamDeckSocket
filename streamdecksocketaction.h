/**********************************************************************
Copyright (C) 2002-2012 DAZ 3D, Inc. All Rights Reserved.

This file is part of the DAZ Studio SDK.

This file may be used only in accordance with the DAZ Studio SDK
license provided with the DAZ Studio SDK.

The contents of this file may not be disclosed to third parties,
copied or duplicated in any form, in whole or in part, without the
prior written permission of DAZ 3D, Inc, except as explicitly
allowed in the DAZ Studio SDK license.

See http://www.daz3d.com to contact DAZ 3D, Inc or for more
information about the DAZ Studio SDK.
**********************************************************************/

#ifndef STREAM_DECK_SOCKET_ACTION_H
#define STREAM_DECK_SOCKET_ACTION_H

/*****************************
Include files
*****************************/
#include <dzaction.h>
#include "actionserver.h"

/*****************************
Class definitions
*****************************/
/**
Our plugin adds a new action class to DAZ Studio. Actions are menu and tool bar
entries that the user can click on. Creating a new action is the easiest way to
extend the DAZ Studio interface from a plugin.

Our new action inherits from DzEditAction so that it will automatically be added
to the 'Edit' menu when the plugin is loaded the first time.
**/
class StreamDeckSocketAction : public DzEditAction {
	Q_OBJECT
public:

	/** Constructor **/
	StreamDeckSocketAction();

protected:
	void init();
	/**
	We need to reimplement this virtual function to perform our 'action'.
	This gets called whenever the user clicks on our action item.
	**/
	virtual void	executeAction();
private:
	ActionServer *m_server;
};

#endif // STREAM_DECK_SOCKET_ACTION_H