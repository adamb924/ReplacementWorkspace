#ifndef READSETTINGS_H
#define READSETTINGS_H

#include <QXmlDefaultHandler>
#include <QWidget>

#include "ReplacementItem.h"

class ReadSettings : public QXmlDefaultHandler
{
public:
	ReadSettings();
	~ReadSettings();

	bool startElement(const QString &namespaceURI,
						const QString &localName,
						const QString &qName,
						const QXmlAttributes &attributes);
	bool endElement(const QString &namespaceURI,
						const QString &localName,
						const QString &qName);
	bool characters(const QString &str);
	bool fatalError(const QXmlParseException &exception);

	QList<ReplacementItem*> list;

private:
	QString currentString;
};

#endif
