#pragma once

#include "qhttpserver/qhttpserverfwd.h"
#include "dzfloatproperty.h"

#include <QtCore/QObject>
#include <dzjsonwriter.h>

/// Greeting

class ActionServer : public QObject
{
	Q_OBJECT

public:
	ActionServer();

private:
	void enumerateActions(DzJsonWriter* json);
	void enumerateCustomActions(DzJsonWriter* json);
	void enumerateDAZActions(DzJsonWriter* json);
	int executeCustomAction(QString action);
	int executeDAZAction(QString action);

	float ActionServer::getPropertyAsPercent(DzFloatProperty* prop);

public slots:
	void handleRequest(QHttpRequest* req, QHttpResponse* resp);
};
