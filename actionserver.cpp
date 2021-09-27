#include "actionserver.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QBuffer>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostAddress>

#include "qhttpserver/qhttpserver.h"
#include "qhttpserver/qhttprequest.h"
#include "qhttpserver/qhttpresponse.h"

#include <dzjsonwriter.h>
#include <dzapp.h>
#include <dzmainwindow.h>
#include <dzactionmgr.h>
#include <dzaction.h>

/// Greeting

ActionServer::ActionServer()
{
	QHttpServer *server = new QHttpServer(this);
	connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
		this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
	/*
	 * FYI - Address and Port are #define'd in "qhttpserver.h" 
	 * Default is (QHostAddress::LocalHost, 8080)
	 */
	server->listen(LISTEN_ADDRESS, LISTEN_PORT);
}

void ActionServer::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
	QRegExp exp("^/user/([a-z]+)$");
	QRegExp action("^/action/(.*)$");
	if (exp.indexIn(req->path()) != -1)
	{
		resp->setHeader("Content-Type", "text/html");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->writeHead(200);

		QString name = exp.capturedTexts()[1];
		QString body = tr("<html><head><title>Greeting App</title></head><body><h1>Hello %1!</h1></body></html>");
		resp->end(body.arg(name).toUtf8());
	}
	else if (req->path().startsWith(QString("/enumerate"))) {
		QBuffer *buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter *json = new DzJsonWriter(buf);
		resp->setHeader("Content-Type", "application/json");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->writeHead(200);

		enumerateActions(json);
		buf->close();

		resp->end(buf->buffer());
		delete json;
		delete buf;
	}
	else if (action.indexIn(req->path()) != -1) {
		QString guid = action.capturedTexts()[1];
		int r = executeAction(guid);
		QBuffer *buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter *json = new DzJsonWriter(buf);
		resp->setHeader("Content-Type", "application/json");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->writeHead(200);
		json->startObject();
		json->addMember("value", r);
		json->addMember("status", (r == -1 ? "failed" : "ok"));
		json->finishObject();
		buf->close();

		resp->end(buf->buffer());
		delete json;
		delete buf;

	}
	else
	{
		resp->writeHead(403);
		resp->end(QByteArray("You aren't allowed here!"));
	}
}

int ActionServer::executeAction(QString action)
{
	DzMainWindow    *mw = dzApp->getInterface();
	DzActionMgr		*am = mw->getActionMgr();

	int ix = am->findCustomAction(action);
	if (ix != -1) {
		dzApp->debug(QString("executeAction: %1 <-> %2").arg(am->getCustomActionText(ix)).arg(action));
		DzAction *cu = am->getCustomAction(ix);
		cu->trigger();
	}
	return ix;
}

void ActionServer::enumerateActions(DzJsonWriter *json)
{
	DzMainWindow    *mw = dzApp->getInterface();
	DzActionMgr		*am = mw->getActionMgr();

	json->startArray();
	int nActions = am->getNumCustomActions();
	for (int i = 0; i < nActions; i++) {
		json->startObject();

		QString path = am->getCustomActionIcon(i);
		if ((!path.isEmpty()) && (QFileInfo(path).exists())) {
			json->addMember("icon", path);
		}
		json->addMember("name", am->getCustomActionName(i));
		json->addMember("text", am->getCustomActionText(i));
		json->finishObject();
	}
	json->finishArray();
}

