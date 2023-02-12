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

/*****************************
   Include files
*****************************/
#include <QtGui/QMessageBox>

#include "dzapp.h"
#include "dzappsettings.h"
#include "dzmainwindow.h"

#include "streamdecksocketdlg.h"
#include "streamdecksocketaction.h"
#include "actionserver.h"
#include "settings.h"

///////////////////////////////////////////////////////////////////////
// DzAFirstPluginAction
///////////////////////////////////////////////////////////////////////

/**
**/
StreamDeckSocketAction::StreamDeckSocketAction() :
	DzEditAction( "Stream Deck", "Stream Deck Settings" )
{
}

/**
**/
void StreamDeckSocketAction::init()
{
	m_server = new ActionServer();
}
/**
**/
void StreamDeckSocketAction::executeAction()
{
	// Check if the main window has been created yet.
	// If it hasn't, alert the user and exit early.

	DzMainWindow *mw = dzApp->getInterface();
	if( !mw )
	{
		QMessageBox::warning( 0, tr("Error"),
			tr("The main window has not been created yet."), QMessageBox::Ok );

		return;
	}

	// Create and show the dialog. If the user cancels, exit early,
	// otherwise continue on and do the thing that required modal
	// input from the user.

	StreamDeckSocketDlg *dlg = new StreamDeckSocketDlg( mw );
	if( dlg->exec() != QDialog::Accepted )
	{
		return;
	}

	DzAppSettings( STREAM_DECK_SETTINGS_NAME ).setIntValue( STREAM_DECK_SETTINGS_PORT_NAME, dlg->getPort() );
	m_server->restart();
}
