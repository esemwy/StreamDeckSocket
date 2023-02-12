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
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

#include "dzappsettings.h"
#include "dzstyle.h"

#include "streamdecksocketdlg.h"
#include "settings.h"

/*****************************
   Local definitions
*****************************/
#define STREAM_DECK_SOCKET_NAME		"StreamDeckSocket"
#define STREAM_DECK_PORT_MIN	1025
#define STREAM_DECK_PORT_MAX	65535

///////////////////////////////////////////////////////////////////////
// StreamDeckSocketDlt
///////////////////////////////////////////////////////////////////////

/**
**/
StreamDeckSocketDlg::StreamDeckSocketDlg( QWidget *parent ) :
	DzBasicDialog( parent, STREAM_DECK_SOCKET_NAME)
{
	// Declarations
	int margin = style()->pixelMetric( DZ_PM_GeneralMargin );
	int wgtHeight = style()->pixelMetric( DZ_PM_ButtonHeight );
	int btnMinWidth = style()->pixelMetric( DZ_PM_ButtonMinWidth );

	// Set the dialog title
	setWindowTitle( tr("Stream Deck Settings") );

	// Create dialog contents
	QWidget* wgt = new QWidget( this );
	QHBoxLayout *hbox = new QHBoxLayout( wgt );

	QLabel *lbl = new QLabel( tr("Port :"), wgt);
	lbl->setFixedHeight( wgtHeight );
	lbl->setMinimumWidth( btnMinWidth );

	this->sbPort = new QSpinBox( wgt );
	this->sbPort->setRange( STREAM_DECK_PORT_MIN, STREAM_DECK_PORT_MAX );
	this->sbPort->setValue( DzAppSettings( STREAM_DECK_SETTINGS_NAME ).getIntValue( STREAM_DECK_SETTINGS_PORT_NAME, STREAM_DECK_DEFAULT_PORT ));
	this->sbPort->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Ignored );
	this->sbPort->setFixedHeight( wgtHeight );
	this->sbPort->setMinimumWidth( btnMinWidth );

	QPushButton *btn = new QPushButton( tr("Default"), wgt);
	btn->setFixedHeight( wgtHeight );
	btn->setMinimumWidth( btnMinWidth );

	// Connect button
	connect( btn, SIGNAL( released() ), this, SLOT( useDefaultPort() ));

	// Add the widgets to the basic dialog
	hbox->addWidget( lbl );
	hbox->addWidget( this->sbPort );
	hbox->addWidget( btn );
	wgt->setLayout( hbox );
	addWidget( wgt );

	// Make the dialog fit its contents, with a minimum width, and lock it down
	resize( QSize( 300, 0 ).expandedTo(minimumSizeHint()) );
	setFixedWidth( width() );
	setFixedHeight( height() );
}

uint16_t StreamDeckSocketDlg::getPort()
{
	return this->sbPort->value();
}

void StreamDeckSocketDlg::useDefaultPort()
{
	this->sbPort->setValue(STREAM_DECK_DEFAULT_PORT);
}