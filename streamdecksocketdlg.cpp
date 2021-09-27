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
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QToolTip>
#include <QtGui/QWhatsThis>

#include "dzapp.h"
#include "dzstyle.h"
#include "dzmainwindow.h"
#include "dzactionmgr.h"
#include "dzaction.h"

#include "streamdecksocketdlg.h"

/*****************************
   Local definitions
*****************************/
#define STREAM_DECK_SOCKET_NAME		"StreamDeckSocket"

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
	setWindowTitle( tr("Stream Deck : Dialog") );

	//... more widgets/layouts here
	QLabel *lbl = new QLabel( tr("Hello, World!"), this );
	lbl->setObjectName( QString("%1DlgHelloWorldLbl").arg(STREAM_DECK_SOCKET_NAME) );
	lbl->setFixedHeight( wgtHeight );
	lbl->setMinimumWidth( btnMinWidth );

	// Add the widget to the basic dialog
	addWidget( lbl );

	// Make the dialog fit its contents, with a minimum width, and lock it down
	resize( QSize( 300, 0 ).expandedTo(minimumSizeHint()) );
	setFixedWidth( width() );
	setFixedHeight( height() );
}
