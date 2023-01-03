##include "actionserver.h"

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
#include "dzbone.h"
#include "dzscene.h"
#include <dzfloatproperty.h>

/// Greeting

ActionServer::ActionServer()
{
	QHttpServer* server = new QHttpServer(this);
	connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
		this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
	/*
	 * FYI - Address and Port are #define'd in "qhttpserver.h"
	 * Default is (QHostAddress::LocalHost, 8080)
	 */
	server->listen(LISTEN_ADDRESS, LISTEN_PORT);
}

void ActionServer::handleRequest(QHttpRequest* req, QHttpResponse* resp)
{
	QRegExp exp("^/user/([a-z]+)$");
	QRegExp action("^/action/(.*)$");
	QRegExp dazaction("^/dazaction/(.*)$");

	QRegExp transform("^/transform/(.*)$");
	QRegExp toDefault("^/toDefault/(.*)$");
	QRegExp setSensitivity("^/setSensitivity/(.*)$");
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
		QBuffer* buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter* json = new DzJsonWriter(buf);
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
		int r = executeCustomAction(guid);
		QBuffer* buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter* json = new DzJsonWriter(buf);
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
	else if (dazaction.indexIn(req->path()) != -1) {
		QString name = dazaction.capturedTexts()[1];
		int r = executeDAZAction(name);
		QBuffer* buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter* json = new DzJsonWriter(buf);
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
	else if (transform.indexIn(req->path()) != -1) {
		//handle incoming instructions.  Format is "[action]_[X]_[Y]_[Z]"
		QString cmdLine = transform.capturedTexts()[1];
		dzApp->log(QString("transform instructions: (%1) ").arg(cmdLine));
		QStringList cmds = cmdLine.split('_');

		DzNode* node = dzScene->getPrimarySelection();
		if (!node) {
			resp->writeHead(404);
			resp->end(QByteArray("No node selected!"));
			return;  //no node selected then bail
		}

		float deltaX = cmds[1].toFloat();
		float deltaY = cmds[2].toFloat();
		float deltaZ = cmds[3].toFloat();
		DzFloatProperty* xTransform, * yTransform, * zTransform;

		if (cmds[0] == "Translate") {
			xTransform = node->getXPosControl();
			yTransform = node->getYPosControl();
			zTransform = node->getZPosControl();
		}
		else if (cmds[0] == "Scale") {
			xTransform = node->getXScaleControl();
			yTransform = node->getYScaleControl();
			zTransform = node->getZScaleControl();
		}
		else {// default to rotate. Omitting the following code: if (cmds[0] == "Rotate") { 
			xTransform = node->getXRotControl();
			yTransform = node->getYRotControl();
			zTransform = node->getZRotControl();
		}

		xTransform->setValue(xTransform->getValue() + (deltaX * xTransform->getSensitivity()));
		yTransform->setValue(yTransform->getValue() + (deltaY * yTransform->getSensitivity()));
		zTransform->setValue(zTransform->getValue() + (deltaZ * zTransform->getSensitivity()));


		//Return info on the current values.  
		//todo Could be refactored into a seperate fuction since it's repeated a couple of times.
		QBuffer* buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter* json = new DzJsonWriter(buf);
		resp->setHeader("Content-Type", "application/json");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->writeHead(200);
		json->startObject();
		json->addMember("Label", node->getLabel());
		json->addMember("X-Value", xTransform->getValue());
		json->addMember("X-Percent", getPropertyAsPercent(xTransform));
		json->addMember("Y-Value", yTransform->getValue());
		json->addMember("Y-Percent", getPropertyAsPercent(zTransform));
		json->addMember("Z-Value", zTransform->getValue());
		json->addMember("Z-Percent", getPropertyAsPercent(zTransform));
		json->finishObject();
		buf->close();

		resp->end(buf->buffer());
		delete json;
		delete buf;
	}
	else if (toDefault.indexIn(req->path()) != -1) {  //This sets the value to the default value listed in the node's properties
		//handle incoming instructions.  Format is "[action]_[X]_[Y]_[Z]"
		QString cmdLine = toDefault.capturedTexts()[1];
		dzApp->log(QString("transform instructions: (%1) ").arg(cmdLine));
		QStringList cmds = cmdLine.split('_');

		DzNode* node = dzScene->getPrimarySelection();
		if (!node) {
			resp->writeHead(404);
			resp->end(QByteArray("No node selected!"));
			return;  //no node selected then bail
		}


		DzFloatProperty* xTransform, * yTransform, * zTransform;


		if (cmds[0] == "Translate") {
			xTransform = node->getXPosControl();
			yTransform = node->getYPosControl();
			zTransform = node->getZPosControl();
		}
		else if (cmds[0] == "Scale") {
			xTransform = node->getXScaleControl();
			yTransform = node->getYScaleControl();
			zTransform = node->getZScaleControl();
		}
		else {// default to rotate. Omitting the following code: if (cmds[0] == "Rotate") { 
			xTransform = node->getXRotControl();
			yTransform = node->getYRotControl();
			zTransform = node->getZRotControl();
		}
		if (cmds[1] == "Y") xTransform->setValue(xTransform->getDefaultValue());
		if (cmds[2] == "Y") yTransform->setValue(yTransform->getDefaultValue());
		if (cmds[3] == "Y") zTransform->setValue(zTransform->getDefaultValue());

		//Return info on the current values.  //todo Could be refactored into a seperate fuction since it's repeated a couple of times.
		QBuffer* buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter* json = new DzJsonWriter(buf);
		resp->setHeader("Content-Type", "application/json");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->writeHead(200);
		json->startObject();
		json->addMember("Label", node->getLabel());
		json->addMember("X-Value", xTransform->getValue());
		json->addMember("X-Percent", xTransform->getDisplayAsPercent());
		json->addMember("Y-Value", yTransform->getValue());
		json->addMember("Y-Percent", yTransform->getDisplayAsPercent());
		json->addMember("Z-Value", zTransform->getValue());
		json->addMember("Z-Percent", zTransform->getDisplayAsPercent());
		json->finishObject();
		buf->close();

		resp->end(buf->buffer());
		delete json;
		delete buf;
	}
	else if (setSensitivity.indexIn(req->path()) != -1) {
		//handle incoming instructions.  Format is "[action]_[X]_[Y]_[Z]"
		QString cmdLine = setSensitivity.capturedTexts()[1];
		dzApp->log(QString("SetSensitivity: (%1) ").arg(cmdLine));
		QStringList cmds = cmdLine.split('_');

		DzNode* node = dzScene->getPrimarySelection();
		if (!node) {
			resp->writeHead(404);
			resp->end(QByteArray("No node selected!"));
			return;  //no node selected then bail
		}
		float outX, outY, outZ;
		float deltaX = cmds[1].toFloat();
		float deltaY = cmds[2].toFloat();
		float deltaZ = cmds[3].toFloat();
		DzFloatProperty* xTransform, * yTransform, * zTransform;

		if (cmds[0] == "Translate") {
			xTransform = node->getXPosControl();
			yTransform = node->getYPosControl();
			zTransform = node->getZPosControl();
		}
		else if (cmds[0] == "Scale") {
			xTransform = node->getXScaleControl();
			yTransform = node->getYScaleControl();
			zTransform = node->getZScaleControl();
		}
		else {// default to rotate, otherwise the _Transform variables may be unassigned. Omitting the following code: if (cmds[0] == "Rotate") { 
			xTransform = node->getXRotControl();
			yTransform = node->getYRotControl();
			zTransform = node->getZRotControl();
		}
		outX = xTransform->getSensitivity() + (deltaX * 0.5);
		outY = xTransform->getSensitivity() + (deltaY * 0.5);
		outZ = xTransform->getSensitivity() + (deltaZ * 0.5);


		xTransform->setSensitivity((outX > 0) ? outX : 0.5);
		yTransform->setSensitivity((outY > 0) ? outY : 0.5);
		zTransform->setSensitivity((outZ > 0) ? outZ : 0.5);


		//Return info on the current values.  
		//todo Could be refactored into a seperate fuction since it's repeated a couple of times.
		QBuffer* buf = new QBuffer();
		buf->open(QIODevice::WriteOnly);
		DzJsonWriter* json = new DzJsonWriter(buf);
		resp->setHeader("Content-Type", "application/json");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->writeHead(200);
		json->startObject();
		json->addMember("Label", node->getLabel());
		json->addMember("X-Sensitivity", xTransform->getSensitivity());
		json->addMember("X-Percent", xTransform->getDisplayAsPercent());
		json->addMember("Y-Sensitivity", yTransform->getSensitivity());
		json->addMember("Y-Percent", yTransform->getDisplayAsPercent());
		json->addMember("Z-Sensitivity", zTransform->getSensitivity());
		json->addMember("Z-Percent", zTransform->getDisplayAsPercent());
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

int ActionServer::executeCustomAction(QString action)
{
	DzMainWindow* mw = dzApp->getInterface();
	DzActionMgr* am = mw->getActionMgr();

	int ix = am->findCustomAction(action);
	if (ix != -1) {
		dzApp->debug(QString("executeCustomAction: %1 <-> %2").arg(am->getCustomActionText(ix)).arg(action));
		DzAction* cu = am->getCustomAction(ix);
		cu->trigger();
	}
	return ix;
}

int ActionServer::executeDAZAction(QString action)
{
	DzMainWindow* mw = dzApp->getInterface();
	DzActionMgr* am = mw->getActionMgr();

	DzAction* ac = am->findAction(action);
	if (ac) {
		dzApp->debug(QString("executeDAZAction: %1").arg(action));
		ac->trigger();
		return 0;
	}
	else {
		dzApp->debug(QString("Could not find DAZ Action <%1>").arg(action));
		return -1;
	}
	return 0;
}

void ActionServer::enumerateActions(DzJsonWriter* json)
{
	json->startArray();
	enumerateCustomActions(json);
	enumerateDAZActions(json);
	json->finishArray();
}

void ActionServer::enumerateCustomActions(DzJsonWriter* json)
{
	DzMainWindow* mw = dzApp->getInterface();
	DzActionMgr* am = mw->getActionMgr();

	int nActions = am->getNumCustomActions();
	for (int i = 0; i < nActions; i++) {
		json->startObject();

		QString path = am->getCustomActionIcon(i);
		if ((!path.isEmpty()) && (QFileInfo(path).exists())) {
			json->addMember("icon", path);
		}
		json->addMember("name", am->getCustomActionName(i));
		json->addMember("text", am->getCustomActionText(i));
		json->addMember("custom", true);
		json->finishObject();
	}
}

void ActionServer::enumerateDAZActions(DzJsonWriter* json)
{
	DzMainWindow* mw = dzApp->getInterface();
	DzActionMgr* am = mw->getActionMgr();

	int nActions = am->getNumActions();
	for (int i = 0; i < nActions; i++) {
		json->startObject();
		DzAction* ac = am->getAction(i);
		QString path = ac->getIconFile();
		if ((!path.isEmpty()) && (QFileInfo(path).exists())) {
			json->addMember("icon", path);
		}

		json->addMember("name", QString(ac->metaObject()->className()));
		json->addMember("text", ac->text());
		json->addMember("custom", false);
		json->finishObject();
	}
}
float ActionServer::getPropertyAsPercent(DzFloatProperty* prop) {
	float myValue = prop->getValue();
	float myMin = prop->getMin();
	float myMax = prop->getMax();
	bool isNeg = myValue < 0;
	float myOutput = 0.0f;
	if (isNeg) {
		myOutput = myValue / myMin;
	}
	else
	{
		myOutput = myValue / myMax;
	}
	return (myOutput*100);
}
