#pragma once

#include "qhttpserver/qhttpserverfwd.h"

#include <QtCore/QObject>
#include <dzjsonwriter.h>

/// Greeting

class ActionServer : public QObject
{
	Q_OBJECT

public:
	ActionServer();

private:
	void enumerateActions(DzJsonWriter *json);
	int executeAction(QString action);

public slots:
	void handleRequest(QHttpRequest *req, QHttpResponse *resp);
};